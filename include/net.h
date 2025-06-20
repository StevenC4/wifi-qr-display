#pragma once

#include <Arduino.h>

struct WifiData {
  String ssid;
  String password;
  unsigned long fetchedAt = 0;
  unsigned long validUntil = 0;
  bool valid = false;
  bool isStatic = false;

  WifiData() = default;
  WifiData(String s, String p) : ssid(std::move(s)), password(std::move(p)), fetchedAt(0), valid(false) {}

  String getQr() const {
    return "WIFI:T:WPA;S:" + ssid + ";P:" + password + ";;";
  }
};


WifiData fetchWifiData();
