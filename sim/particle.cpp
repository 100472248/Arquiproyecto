//
// Created by jaime-vaquero on 2/10/23.
//

#include "particle.hpp"
#include <vector>
Particle::Particle(int id, std::vector<double> position, std::vector<double> speed_gradient, std::vector<double> speed) {
  m_id = id;
  m_position = position;
  m_speed_gradient = speed_gradient;
  m_speed = speed;
}

void Particle::Set_acceleration(std::vector<double> acceleration) {
  m_acceleration = acceleration;
}

void Particle::Set_density(double density) {
  m_density = density;
}