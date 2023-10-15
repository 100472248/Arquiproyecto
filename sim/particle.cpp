//
// Created by jaime-vaquero on 2/10/23.
//

#include "particle.hpp"
#include <vector>
#include <array>

Particle::Particle(int id, std::vector<double> position, std::vector<double> speed_gradient, std::vector<double> speed) {
  m_id = id;
  m_position = position;
  m_speed_gradient = speed_gradient;
  m_speed = speed;

}
void Particle::Set_bloque(std::array<int, 3> bloque) {
    m_bloque = bloque;
}

void Particle::Set_acceleration(std::array<double, 3> acceleration) {
  m_acceleration = acceleration;
}

void Particle::Set_density(double density) {
  m_density = density;
}

void Particle::Set_mass(double mass){
  m_mass = mass;
}

std::array<int, 3> Particle::get_bloque() {
    return m_bloque;
}

std::vector<double> Particle::get_position() {
    return m_position;
}

std::array<double, 3> Particle::get_acceleration(){
    return m_acceleration;
}

double Particle::get_mass() {
    return m_mass;
}

std::vector<double> Particle:: get_speed(){
    std::vector<double> speed = {m_speed[0], m_speed[1], m_speed[2]};
    return speed;
}

double Particle::update_density(double aumento) {
    m_density += aumento;
    return get_density();
}

double Particle::get_density() {
    return m_density;
}

void Particle::update_acceleration(std::array<double, 3> aumento) {
    m_acceleration = {m_acceleration[0] + aumento[0], m_acceleration[1] + aumento[1], m_acceleration[2] + aumento[2]};
}


