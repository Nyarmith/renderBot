#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <functional>

const sf::Vector2u g_screenDim {800,640};

float rotation=1.0f;
glm::vec3 rotation_axis {.7071,.7071,0};
const std::vector<glm::vec3> g_vertices {{1,1,1},{-1,1,-1},{-1,-1,1},{1,-1,-1}};
const std::vector<float> g_indices {0,1,2,0,1,3,0,2,3,1,2,3};
const glm::vec3 g_cameraPos {0,0,-4};
const glm::vec3 g_cameraGaze {0,0,1};


void init()
{
}

void poll(sf::RenderWindow &win)
{
}

void update(float dt)
{
  float twoPi = 2*glm::pi<float>();
  rotation += dt/2;
  if (rotation >= twoPi)
    rotation -= twoPi;
}

void draw(sf::RenderWindow &win)
{
  std::vector<glm::vec3> new_vertices;
  new_vertices.reserve(4);
  for (const glm::vec3 &v : g_vertices)
    new_vertices.push_back( glm::rotate(v,rotation,rotation_axis) );

  // the camera, which will be further back on the negative-z,
  // will be pointing towards the positive-z and tracing rays.
  // we will make the shorter dimension of length 1 and the other will scale
  
  //start 1 z in-front of you
  glm::vec3 ray_dir = g_cameraPos + g_cameraGaze;
  glm::vec3 x_step  = -(glm::vec3{1,0,0} / (g_screenDim.x));
  glm::vec3 y_step  = -(glm::vec3{0,1,0} / (g_screenDim.y));
  //now start in the top-left
  ray_dir += glm::vec3(.5,.5,0);

  for (float x=0; x<g_screenDim.x; ++x) {
    for (float y=0; y<g_screenDim.y; ++y) {
      glm::vec3 this_steps_gaze = glm::normalize(ray_dir + x*x_step + y*y_step);
      //do I intersect
      if ( rayIntersects(g_cameraPos, this_steps_gaze) ) {
        glm::vec3 col = rayColor(this_steps_gaze);
        //draw color on screen
      }
  }
}

int main()
{
  sf::RenderWindow win(sf::VideoMode(g_screenDim.x, g_screenDim.y), "Raytracer Demo");

  sf::Clock timer;
  init();
  while( win.isOpen() )
  {
    poll(win);
    update(timer.getElapsedTime().asSeconds());
    timer.restart();
    win.clear(sf::Color::Black);
    draw(win);
  }
}
