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

void Particle::Set_mass(double mass){
  m_mass = mass;
}
std::vector<double> Particle:: get_speed(){
    std::vector<double> speed = {m_speed[0], m_speed[1], m_speed[2]};
    return speed;
}
std::vector<double> Particle:: get_position(){
    std::vector<double> pos = {m_position[0], m_position[1], m_position[2]};
    return pos;
}