#pragma once

#include <stdint.h>

#include <memory>
#include <thread>
#include <vector>

#include <rtl-sdr.h>

#include "publisher.h"
#include "types.h"

class RTLSDR {
public:
  static std::unique_ptr<RTLSDR> open(uint32_t index = 0);

  explicit RTLSDR(rtlsdr_dev_t* dev);
  ~RTLSDR();

  const std::vector<int>& getTunerGains() {
    return tunerGains_;
  }

  void setCenterFrequency(uint32_t freq);
  void setSampleRate(uint32_t rate);
  void setTunerGain(int gain);
  void setPublisher(std::unique_ptr<Publisher> publisher);

  void start(const std::shared_ptr<Queue<Samples> >& queue);
  void stop();
  void handle(unsigned char* buf, uint32_t len);

protected:
  rtlsdr_dev_t* dev_;

  std::vector<int> tunerGains_;
  std::thread thread_;

  // Set on start; cleared on stop
  std::shared_ptr<Queue<Samples> > queue_;

  // Optional publisher for samples
  std::unique_ptr<Publisher> publisher_;
};