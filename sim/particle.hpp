//
// Created by jaime-vaquero on 2/10/23.
//

#ifndef ARQUIPROYECTO_PARTICLE_HPP
#define ARQUIPROYECTO_PARTICLE_HPP

#include <vector>
#include <array>

class Particle {
  private:
    int m_id;
    std::vector<double> m_position;
    std::vector<double> m_speed_gradient;
    std::vector<double> m_speed;
    std::array<double, 3>  m_acceleration{};
    double m_density{};
    double m_mass{};
    std::array<int, 3> m_bloque{};
  public:
    Particle(int id, std::vector<double> position, std::vector<double> speed_gradient, std::vector<double> speed);
    void Set_acceleration(std::array<double, 3> acceleration);
    void Set_density(double density);
    void Set_mass(double mass);
    void Set_bloque(std::array<int, 3> bloque);
    std::array<int, 3> get_bloque();
    std::vector<double> get_position();
    std::vector<double>get_speed();
    double get_mass() const;
    double get_density() const;
    int get_id() const;
    void update_density(double aumento);
    void update_acceleration(std::array<double, 3> aumento);
    std::array<double, 3> get_acceleration();
    std::vector<double> get_gradient();
    void set_gradient(std::vector<double> gradient_part);
    void set_speed(std::vector<double> speed_part);
    void set_position(std::vector<double> position_part);
};

#endif //ARQUIPROYECTO_PARTICLE_HPP