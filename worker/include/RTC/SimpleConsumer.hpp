#ifndef MS_RTC_SIMPLE_CONSUMER_HPP
#define MS_RTC_SIMPLE_CONSUMER_HPP

#include "RTC/Codecs/PayloadDescriptorHandler.hpp"
#include "RTC/Consumer.hpp"
#include "RTC/RtpDataCounter.hpp"
#include "RTC/SeqManager.hpp"

namespace RTC
{
	class SimpleConsumer : public Consumer
	{
	public:
		SimpleConsumer(const std::string& id, Listener* listener, json& data);
		~SimpleConsumer() override;

	public:
		void FillJson(json& jsonObject) const override;
		void FillJsonStats(json& jsonArray) const override;
		void HandleRequest(Channel::Request* request) override;
		bool IsHealthy() const override;
		void TransportConnected() override;
		void ProducerRtpStreamHealthy(RTC::RtpStream* rtpStream, uint32_t mappedSsrc) override;
		void ProducerRtpStreamUnhealthy(RTC::RtpStream* rtpStream, uint32_t mappedSsrc) override;
		void SendRtpPacket(RTC::RtpPacket* packet) override;
		void GetRtcp(RTC::RTCP::CompoundPacket* packet, uint64_t now) override;
		void ReceiveNack(RTC::RTCP::FeedbackRtpNackPacket* nackPacket) override;
		void ReceiveKeyFrameRequest(RTC::RTCP::FeedbackPs::MessageType messageType) override;
		void ReceiveRtcpReceiverReport(RTC::RTCP::ReceiverReport* report) override;
		uint32_t GetTransmissionRate(uint64_t now) override;
		float GetLossPercentage() const override;

	protected:
		void Started() override;
		void Paused(bool wasProducer = false) override;
		void Resumed(bool wasProducer = false) override;

	private:
		void CreateRtpStream();
		void RequestKeyFrame();
		void RetransmitRtpPacket(RTC::RtpPacket* packet);

	private:
		// Allocated by this.
		RTC::RtpStreamSend* rtpStream{ nullptr };
		// Others.
		bool keyFrameSupported{ false };
		bool syncRequired{ true };
		uint64_t lastRtcpSentTime{ 0 };
		uint16_t maxRtcpInterval{ 0 };
		RTC::SeqManager<uint16_t> rtpSeqManager;
		RTC::SeqManager<uint32_t> rtpTimestampManager;
		std::unique_ptr<RTC::Codecs::EncodingContext> encodingContext;
		RTC::RtpDataCounter retransmittedCounter;
		RTC::RtpStream* producerRtpStream{ nullptr };
	};
} // namespace RTC

#endif
