//
// Created by jaime-vaquero on 2/10/23.
//

#ifndef ARQUIPROYECTO_PARTICLE_HPP
#define ARQUIPROYECTO_PARTICLE_HPP

#include <vector>

class Particle {
private:
    int m_id;
    std::vector<double> m_position;
    std::vector<double> m_speed_gradient;
    std::vector<double> m_speed;
    std::vector<double> m_acceleration;
    double m_density;
public:
    Particle(int id, std::vector<double> position, std::vector<double> speed_gradient, std::vector<double> speed);
    void Set_acceleration(std::vector<double> acceleration);
    void Set_density(double density);
};

#endif //ARQUIPROYECTO_PARTICLE_HPP
