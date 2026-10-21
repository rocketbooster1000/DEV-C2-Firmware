// coulomb_counter.hpp
// Tracks battery charge by integrating current over time (Q = I * dt).
// Positive current means the battery is discharging.

#pragma once

class CoulombCounter
{
public:
  // max_coulombs: full battery capacity in coulombs (Ah * 3600)
  CoulombCounter(double max_coulombs);

  // Call periodically with the latest current (mA) and millis() timestamp (ms)
  void update(long current_mA, unsigned long now_ms);

  double get_charge_coulombs();
  double get_soc_percent();

  // Setters return false (and change nothing) if the value is out of range
  bool set_soc_percent(double percent);
  bool set_charge_coulombs(double charge_coulombs);
  bool set_max_coulombs(double max_coulombs);

private:
  double m_charge_coulombs;
  double m_max_coulombs;
  unsigned long m_prev_time_ms;
  bool m_started; // false until the first update() records a timestamp
};
