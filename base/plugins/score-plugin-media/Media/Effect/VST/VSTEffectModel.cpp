#include "VSTEffectModel.hpp"

#include <QUrl>
#include <QFile>
#include <QFileInfo>

#include <ModernMIDI/midi_message.h>
#include <set>
#include <iostream>
#include <memory>
#include <cmath>
#include <unordered_map>
#include <ossia/detail/math.hpp>
#include <score/tools/Todo.hpp>
#include <ossia/detail/logger.hpp>
#include <ossia/dataflow/fx_node.hpp>
#include <ossia/dataflow/graph_node.hpp>
#include <ossia/network/domain/domain.hpp>
#include <Media/ApplicationPlugin.hpp>
#include <Media/Effect/VST/VSTNode.hpp>
#include <aeffectx.h>
#include <QTimer>
#include <Process/Dataflow/Port.hpp>
#include <Engine/Executor/DocumentPlugin.hpp>
#include <websocketpp/base64/base64.hpp>
void show_vst2_editor(AEffect& effect, ERect rect);
void hide_vst2_editor(AEffect& effect);

namespace Media
{
namespace VST
{
class VSTControlInlet : public Process::ControlInlet
{
  public:
    SCORE_SERIALIZE_FRIENDS
    template<typename T>
    VSTControlInlet(T&& vis, QObject* parent): ControlInlet{vis, parent}
    {
//      vis.writeTo(*this);
    }

    int fxNum{};
};
// Taken from VST API
struct HostCanDos
{
    static const constexpr auto canDoSendVstEvents = "sendVstEvents"; ///< Host supports send of Vst events to plug-in
    static const constexpr auto canDoSendVstMidiEvent = "sendVstMidiEvent"; ///< Host supports send of MIDI events to plug-in
    static const constexpr auto canDoSendVstTimeInfo = "sendVstTimeInfo"; ///< Host supports send of VstTimeInfo to plug-in
    static const constexpr auto canDoReceiveVstEvents = "receiveVstEvents"; ///< Host can receive Vst events from plug-in
    static const constexpr auto canDoReceiveVstMidiEvent = "receiveVstMidiEvent"; ///< Host can receive MIDI events from plug-in
    static const constexpr auto canDoReportConnectionChanges = "reportConnectionChanges"; ///< Host will indicates the plug-in when something change in plug-in�s routing/connections with #suspend/#resume/#setSpeakerArrangement
    static const constexpr auto canDoAcceptIOChanges = "acceptIOChanges"; ///< Host supports #ioChanged ()
    static const constexpr auto canDoSizeWindow = "sizeWindow"; ///< used by VSTGUI
    static const constexpr auto canDoOffline = "offline"; ///< Host supports offline feature
    static const constexpr auto canDoOpenFileSelector = "openFileSelector"; ///< Host supports function #openFileSelector ()
    static const constexpr auto canDoCloseFileSelector = "closeFileSelector"; ///< Host supports function #closeFileSelector ()
    static const constexpr auto canDoStartStopProcess = "startStopProcess"; ///< Host supports functions #startProcess () and #stopProcess ()
    static const constexpr auto canDoShellCategory = "shellCategory"; ///< 'shell' handling via uniqueID. If supported by the Host and the Plug-in has the category #kPlugCategShell
    static const constexpr auto canDoSendVstMidiEventFlagIsRealtime = "sendVstMidiEventFlagIsRealtime"; ///< Host supports flags for #VstMidiEvent


    static const constexpr auto canDoHasCockosViewAsConfig = "hasCockosViewAsConfig"; ///< Host supports flags for #VstMidiEvent

};


struct maker
{
    std::vector<int> controls;
    auto make_inlet(int i, QObject* parent) const
    {
      return new Process::Inlet(Id<Process::Port>(i), parent);

    }
    auto make_control(int i, QObject* parent) const
    {
      return new Process::ControlInlet{Id<Process::Port>{i}, parent};
    }
    auto make_vst_control(int ctrl_id, int i, QObject* parent) const
    {
      return new VSTControlInlet{Id<Process::Port>{i}, parent};
    }
    auto make_outlet(int i, QObject* parent) const
    {
      return new Process::Outlet(Id<Process::Port>(i), parent);
    }
};

struct datastream_maker
{
    DataStreamWriter& out;
    std::vector<int> controls;
    auto make_inlet(int i, QObject* parent) const
    {
      return new Process::Inlet(out, parent);

    }
    auto make_control(int i, QObject* parent) const
    {
      return new Process::ControlInlet{out, parent};
    }
    auto make_vst_control(int ctrl_id, int i, QObject* parent) const
    {
      return new VSTControlInlet{out, parent};
    }
    auto make_outlet(int i, QObject* parent) const
    {
      return new Process::Outlet(out, parent);
    }
};

struct json_maker
{
    const QJsonArray& inlets;
    const QJsonArray& outlets;
    std::vector<int> controls;
    auto make_inlet(int i, QObject* parent) const
    {
      return new Process::Inlet(JSONObjectWriter{inlets[i].toObject()}, parent);

    }
    auto make_control(int i, QObject* parent) const
    {
      return new Process::ControlInlet{JSONObjectWriter{inlets[i].toObject()}, parent};
    }
    auto make_vst_control(int ctrl_id, int i, QObject* parent) const
    {
      return new VSTControlInlet{JSONObjectWriter{inlets[i].toObject()}, parent};
    }
    auto make_outlet(int i, QObject* parent) const
    {
      return new Process::Outlet(JSONObjectWriter{outlets[i].toObject()}, parent);
    }
};

VSTEffectModel::VSTEffectModel(
    const QString& path,
    const Id<EffectModel>& id,
    QObject* parent):
  EffectModel{id, parent},
  m_effectPath{path}
{
  reload(maker{});
}

VSTEffectModel::~VSTEffectModel()
{
  closePlugin();
}


QString VSTEffectModel::prettyName() const
{
  return metadata().getLabel();
}

static auto HostCallback (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
  VstIntPtr result = 0;

  switch (opcode)
  {
    case audioMasterGetTime:
    {
      auto vst = reinterpret_cast<VSTEffectModel*>(effect->resvd1);
      if(vst)
      {
        result = reinterpret_cast<VstIntPtr>(&vst->fx->info);
      }
      break;
    }
    case audioMasterProcessEvents:
      break;
    case audioMasterIOChanged:
      break;
    case audioMasterSizeWindow:
      break;
    case audioMasterGetInputLatency:
      break;
    case audioMasterGetOutputLatency:
      break;
    case audioMasterIdle:
      effect->dispatcher(effect, effEditIdle, 0, 0, nullptr, 0);
      break;
    case audioMasterVersion :
      result = kVstVersion;
      break;

    case audioMasterCurrentId:
      result = effect->uniqueID;
      break;

    case audioMasterGetSampleRate:
      break;

    case audioMasterGetBlockSize:
      break;

    case audioMasterGetCurrentProcessLevel:
      result = kVstProcessLevelUnknown;
      break;

    case audioMasterAutomate:
    {
      auto vst = reinterpret_cast<VSTEffectModel*>(effect->resvd1);
      if(vst)
      {
        auto ctrl_it = vst->controls.find(index);
        if(ctrl_it != vst->controls.end())
        {
          ctrl_it->second->setValue(opt);
        }
        else
        {
          // TODO register control
        }
      }

      break;
    }
    case audioMasterGetAutomationState:
      result = kVstAutomationUnsupported;
      break;

    case audioMasterGetLanguage:
      result = kVstLangEnglish;
      break;

    case audioMasterGetVendorVersion:
      result = 1;
      break;

    case audioMasterGetVendorString:
      std::copy_n("ossia", 6, static_cast<char*>(ptr));
      result = 1;
      break;

    case audioMasterGetProductString:
      std::copy_n("score", 6, static_cast<char*>(ptr));
      result = 1;
      break;

    case audioMasterUpdateDisplay:
      // TODO update all values
      break;
    case audioMasterBeginEdit:
      break;
    case audioMasterEndEdit:
      break;
    case audioMasterOpenFileSelector:
      break;
    case audioMasterCloseFileSelector:
      break;

    case audioMasterCanDo:
    {
      static const std::set<std::string_view> supported{
        HostCanDos::canDoSendVstEvents,
            HostCanDos::canDoSendVstMidiEvent,
            HostCanDos::canDoSendVstTimeInfo,
            HostCanDos::canDoSendVstMidiEventFlagIsRealtime,
            HostCanDos::canDoHasCockosViewAsConfig
      };
      if(supported.find(static_cast<const char*>(ptr)) != supported.end())
        result = 1;
      break;
    }
  }
  return result;
}
void VSTEffectModel::closePlugin()
{
  if(fx)
  {
    hideUI();
    fx = nullptr;
  }
  qDeleteAll(m_inlets);
  qDeleteAll(m_outlets);
  m_inlets.clear();
  m_outlets.clear();
  metadata().setLabel("Dead VST");
}

void VSTEffectModel::showUI()
{
  ERect* vstRect{};

  dispatch(effEditGetRect, 0, 0, &vstRect, 0.0f);

  int w{};
  int h{};
  if(vstRect)
  {
    w = vstRect->right - vstRect->left;
    h = vstRect->bottom - vstRect->top;
  }

  if(w <= 1)
    w = 640;
  if(h <= 1)
    h = 480;

  bool had_ui = bool(ui);
  show_vst2_editor(*fx->fx, *vstRect);
  if(!had_ui && ui)
  {
    auto& ctx = score::IDocument::documentContext(*this);
    connect(&ctx.coarseUpdateTimer, &QTimer::timeout,
            this, [=] {
      dispatch(effEditIdle);
    });
  }
}

void VSTEffectModel::hideUI()
{
  hide_vst2_editor(*fx->fx);
}

template<typename T>
void VSTEffectModel::reload(const T& port_factory)
{
  closePlugin();

  auto path = m_effectPath.toStdString();

  VSTModule* plugin;
  auto& app = score::GUIAppContext().applicationPlugin<Media::ApplicationPlugin>();
  auto it = app.vst_modules.find(path);
  if(it == app.vst_modules.end())
  {
    if(path.empty())
      return;

    bool isFile = QFile(QUrl(m_effectPath).toString(QUrl::PreferLocalFile)).exists();
    if(!isFile)
    {
      qDebug() << "Invalid path: " << m_effectPath;
      return;
    }

    try {
      plugin = new VSTModule{path};
      app.vst_modules.insert({std::move(path), plugin});
    } catch(const std::runtime_error& e) {
      qDebug() << e.what();
      return;
    }
  }
  else
  {
    plugin = it->second;
  }

  if(!plugin)
    return;

  auto main = plugin->getMain();
  if(!main) {
    qDebug() << "plugin has no main";
    return;
  }

  fx = std::make_shared<AEffectWrapper>(main(HostCallback));
  if(!fx->fx)
  {
    qDebug() << "plugin was not created";
    fx.reset();
    return;
  }

  fx->fx->resvd1 = reinterpret_cast<VstIntPtr>(this);

  dispatch(effOpen);

  {
    char buf[256] = {0};
    dispatch(effGetProductString, 0, 0, buf);
    QString s = buf;
    if(!s.isEmpty())
      metadata().setLabel(s);
    else
      metadata().setLabel(QFileInfo(m_effectPath).baseName());
  }

  int inlet_i = 0;
  if(fx->fx->flags & effFlagsIsSynth)
  {
    m_inlets.push_back(port_factory.make_inlet(inlet_i++, this));
    m_inlets[0]->type = Process::PortType::Midi;
  }
  else
  {
    m_inlets.push_back(port_factory.make_inlet(inlet_i++, this));
    m_inlets[0]->type = Process::PortType::Audio;
  }

  // Tempo
  {
    auto tempo = port_factory.make_control(inlet_i++, this);
    m_inlets.push_back(tempo);
    tempo->type = Process::PortType::Message;
    tempo->setCustomData("Tempo");
    tempo->setUiVisible(true);
  }
  {
    // Signature
    auto sig = port_factory.make_control(inlet_i++, this);
    m_inlets.push_back(sig);
    sig->type = Process::PortType::Message;
    sig->setCustomData("Time signature");
    sig->setUiVisible(true);
  }

  for(int i : port_factory.controls)
  {
    auto p = port_factory.make_vst_control(i, inlet_i++, this);

    // Metadata
    {
      auto get_string = [=] (auto req, int i) {
        char paramName[256] = {0};
        dispatch(req, i, 0, paramName);
        return QString::fromUtf8(paramName);
      };
      auto name = get_string(effGetParamName, i);
      auto label = get_string(effGetParamLabel, i);
      // auto display = get_string(effGetParamDisplay, i);

      // Get the nameq
      QString str = name;
      if(!label.isEmpty())
        str += "(" + label + ")";

      p->setCustomData(name);
    }

    // Value
    {
      auto val = fx->getParameter(i);
      p->setDomain(ossia::make_domain(0.f, 1.f));
      p->setValue(val);
      p->hidden = true;
      if(fx->fx->numParams < 10)
      {
        p->setUiVisible(true);
      }

      connect(p, &Process::ControlInlet::valueChanged,
              this, [this,i] (const ossia::value& v){
        auto newval =  ossia::convert<float>(v);
        if(std::abs(newval - fx->getParameter(i)) > 0.0001)
          fx->setParameter(i, newval);
      });
    }

    {
      /*
      VstParameterProperties props;
      auto res = dispatch(effGetParameterProperties, i, 0, &props);
      if(res == 1)
      {
        // apparently there's exactly 0 plug-ins supporting this
        qDebug() << props.label << props.minInteger << props.maxInteger << props.smallStepFloat << props.stepFloat;
      }
      */
    }

    m_inlets.push_back(p);
  }

  m_outlets.push_back(port_factory.make_outlet(0, this));
  m_outlets[0]->type = Process::PortType::Audio;
  m_outlets[0]->setPropagate(true);
}
}
}

template <>
void DataStreamReader::read(
    const Media::VST::VSTEffectModel& eff)
{
  /*
  m_stream << eff.effect();

  m_stream << *eff.m_inlets[0];
  for(int i = 1; i < eff.m_inlets.size(); i++)
    m_stream << *static_cast<Process::ControlInlet*>(eff.m_inlets[i]);

  for(auto v : eff.m_outlets)
    m_stream << *v;

  // TODO save & reload program parameters
  insertDelimiter();
  */
}

template <>
void DataStreamWriter::write(
    Media::VST::VSTEffectModel& eff)
{
  /*
  m_stream >> eff.m_effectPath;
  eff.reload(Media::VST::datastream_maker{*this});
  checkDelimiter();
  */
}

template <>
void JSONObjectReader::read(
    const Media::VST::VSTEffectModel& eff)
{
  /*
  obj["Effect"] = eff.effect();

  QJsonArray inlets;
  inlets.append(toJsonObject(*eff.m_inlets[0]));
  for(int i = 1; i < eff.m_inlets.size(); i++)
    inlets.append(toJsonObject(*static_cast<Process::ControlInlet*>(eff.m_inlets[i])));

  obj["Inlets"] = std::move(inlets);
  obj["Outlets"] = toJsonArray(eff.m_outlets);
  if(eff.fx)
  {
    if(eff.fx->fx->flags & effFlagsProgramChunks)
    {
      void* ptr{};
      auto res = eff.fx->dispatch(effGetChunk, 0, 0, &ptr, 0.f);
      if(ptr && res > 0)
      {
        auto encoded = websocketpp::base64_encode((const unsigned char*)ptr, res);
        obj["Data"] = QString::fromStdString(encoded);
      }
    }
    else
    {
      QJsonArray arr;
      for(int i = 0; i < eff.fx->fx->numParams; i++)
        arr.push_back(eff.fx->getParameter(i));
      obj["Params"] = std::move(arr);
    }
  }
  */
}

template <>
void JSONObjectWriter::write(
    Media::VST::VSTEffectModel& eff)
{
  /*
  eff.m_effectPath = obj["Effect"].toString();
  eff.reload(Media::VST::json_maker{obj["Inlets"].toArray(), obj["Outlets"].toArray()});
  QPointer<Media::VST::VSTEffectModel> ptr = &eff;
  QTimer::singleShot(1000, [obj=this->obj,ptr] {
    if(!ptr)
      return;
    auto& eff = *ptr;
    if(eff.fx)
    {
      if(eff.fx->fx->flags & effFlagsProgramChunks)
      {
        auto it = obj.find("Data");
        if(it != obj.end())
        {
          auto b64 = websocketpp::base64_decode(it->toString().toStdString());
          eff.fx->dispatch(effSetChunk, 0, b64.size(), b64.data(), 0.f);

          for(std::size_t i = 3; i < eff.inlets().size(); i++)
          {
            static_cast<Process::ControlInlet*>(eff.inlets()[i])->setValue(
                  eff.fx->getParameter(i-3));
          }
        }
      }
      else
      {
        auto it = obj.find("Params");
        if(it != obj.end())
        {
          QJsonArray arr = it->toArray();
          for(int i = 0; i < arr.size(); i++)
          {
            eff.fx->setParameter(i, arr[i].toDouble());
          }
        }
      }
    }});
    */
}

Engine::Execution::VSTEffectComponent::VSTEffectComponent(
    Media::VST::VSTEffectModel& proc,
    const Engine::Execution::Context& ctx,
    const Id<score::Component>& id,
    QObject* parent)
  : Engine::Execution::EffectComponent_T<Media::VST::VSTEffectModel>{proc, ctx, id, parent}
{
  AEffect& fx = *proc.fx->fx;
  auto setup_controls = [&] (auto& node) {
    node->ctrl_ptrs.reserve(proc.controls.size());
    for(auto& ctrl : proc.controls)
    {
      auto inlet = ossia::make_inlet<ossia::value_port>();
      node->ctrl_ptrs.push_back({ ctrl.second->fxNum, inlet->data.target<ossia::value_port>(), });
      node->inputs().push_back(std::move(inlet));
    }

    connect(&proc, &Media::VST::VSTEffectModel::controlsChanged,
            this, [=] {
      system().executionQueue.enqueue([] {

      });
    });
  };
  if(fx.flags & effFlagsCanDoubleReplacing)
  {
    if(fx.flags & effFlagsIsSynth)
    {
      auto n = Media::VST::make_vst_fx<true, true>(proc.fx, ctx.plugin.execState.sampleRate);
      setup_controls(n);
      node = std::move(n);
    }
    else
    {
      auto n = Media::VST::make_vst_fx<true, false>(proc.fx, ctx.plugin.execState.sampleRate);
      setup_controls(n);
      node = std::move(n);
    }
  }
  else
  {
    if(fx.flags & effFlagsIsSynth)
    {
      auto n = Media::VST::make_vst_fx<false, true>(proc.fx, ctx.plugin.execState.sampleRate);
      setup_controls(n);
      node = std::move(n);
    }
    else
    {
      auto n = Media::VST::make_vst_fx<false, false>(proc.fx, ctx.plugin.execState.sampleRate);
      setup_controls(n);
      node = std::move(n);
    }
  }
}
