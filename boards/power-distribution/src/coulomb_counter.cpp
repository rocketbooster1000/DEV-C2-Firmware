// coulomb_counter.cpp
// Implementation of the PDB coulomb counter (see coulomb_counter.hpp)

#include "coulomb_counter.hpp"

CoulombCounter::CoulombCounter(double max_coulombs)
{
  m_max_coulombs = max_coulombs;
  m_charge_coulombs = max_coulombs; // assume a full battery at startup
  m_prev_time_ms = 0;
  m_started = false;
}

void CoulombCounter::update(long current_mA, unsigned long now_ms)
{
  // First call: no previous timestamp yet, so just remember this one
  if (!m_started)
  {
    m_prev_time_ms = now_ms;
    m_started = true;
    return;
  }

  // Unsigned subtraction still gives the right dt when millis() wraps around
  unsigned long dt_ms = now_ms - m_prev_time_ms;
  m_prev_time_ms = now_ms;

  // mA * ms = microcoulombs, so divide by 1,000,000 to get coulombs
  m_charge_coulombs -= (double)current_mA * (double)dt_ms / 1000000.0;

  if (m_charge_coulombs < 0.0)
  {
    m_charge_coulombs = 0.0;
  }
  else if (m_charge_coulombs > m_max_coulombs)
  {
    m_charge_coulombs = m_max_coulombs;
  }
}

double CoulombCounter::get_charge_coulombs()
{
  return m_charge_coulombs;
}

double CoulombCounter::get_soc_percent()
{
  return 100.0 * m_charge_coulombs / m_max_coulombs;
}

bool CoulombCounter::set_soc_percent(double percent)
{
  if (percent < 0.0 || percent > 100.0)
  {
    return false;
  }
  m_charge_coulombs = percent / 100.0 * m_max_coulombs;
  return true;
}

bool CoulombCounter::set_charge_coulombs(double charge_coulombs)
{
  if (charge_coulombs < 0.0 || charge_coulombs > m_max_coulombs)
  {
    return false;
  }
  m_charge_coulombs = charge_coulombs;
  return true;
}

bool CoulombCounter::set_max_coulombs(double max_coulombs)
{
  if (max_coulombs <= 0.0)
  {
    return false;
  }
  m_max_coulombs = max_coulombs;

  // Shrinking the capacity may leave the charge above the new max
  if (m_charge_coulombs > m_max_coulombs)
  {
    m_charge_coulombs = m_max_coulombs;
  }
  return true;
}
