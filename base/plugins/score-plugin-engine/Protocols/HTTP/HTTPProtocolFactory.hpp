#pragma once
#include <Protocols/DefaultProtocolFactory.hpp>


namespace Protocols
{
class HTTPProtocolFactory final : public DefaultProtocolFactory
{
  SCORE_CONCRETE("171095cd-6815-4930-be40-5ffe131eb775")
  // Implement with OSSIA::Device
  QString prettyName() const override;

  Device::DeviceInterface* makeDevice(
      const Device::DeviceSettings& settings,
      const score::DocumentContext& ctx) override;
  const Device::DeviceSettings& defaultSettings() const override;

  Device::ProtocolSettingsWidget* makeSettingsWidget() override;

  QVariant
  makeProtocolSpecificSettings(const VisitorVariant& visitor) const override;

  void serializeProtocolSpecificSettings(
      const QVariant& data, const VisitorVariant& visitor) const override;

  bool checkCompatibility(
      const Device::DeviceSettings& a,
      const Device::DeviceSettings& b) const override;
};
}
