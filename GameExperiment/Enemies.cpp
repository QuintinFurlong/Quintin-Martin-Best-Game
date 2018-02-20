#include "Enemies.h"

Enemies::Enemies()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		sf::RectangleShape tempBody;
		tempBody.setSize(sf::Vector2f(100, 150));
		tempBody.setFillColor(sf::Color::Red);
		alive[i] = false;
		velo[i] = sf::Vector2f(0, 0);
		tempBody.setOutlineThickness(1);
		tempBody.setOutlineColor(sf::Color::White);
		bodies.push_back(tempBody);
		toTheRight[i] = true;
	}
}

void Enemies::create(std::vector<EnemyData> t_enemyData)
{
	for (int i = 0; i < t_enemyData.size(); i++)
	{
		bodies[i].setPosition(t_enemyData.at(i).m_position);
		alive[i] = true;
	}
	bodies.resize(t_enemyData.size());
	realMax = t_enemyData.size();
}
/// <summary>
/// 
/// </summary>
/// <param name="t_player"></param>
/// <param name="t_blocks"></param>
/// <param name="t_bullets"></param>
/// <param name="t_bulletVelo"></param>
/// <returns></returns>true if enemy is hit by the players bullet
bool Enemies::update(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets, std::vector<sf::Vector2f> t_bulletVelo)
{
	bool temp = false;
	for (int enmies = 0; enmies < bodies.size(); enmies++)
	{
		for (int bullies = 0; bullies < t_bullets.size(); bullies++)
		{
			if (bodies[enmies].getGlobalBounds().intersects(t_bullets.at(bullies).getGlobalBounds()))
			{
				velo[enmies] += sf::Vector2f(t_bulletVelo.at(bullies).x, t_bulletVelo.at(bullies).y);
				alive[enmies] = false;
				temp = true;
				if (t_bulletVelo.at(bullies).x < 0 && (bodies[enmies].getRotation() != 90 && bodies[enmies].getRotation() != 270))
				{
					toTheRight[enmies] = false;
				}
			}
		}
		velo[enmies] -= gun[enmies].update(t_player, bodies[enmies], alive[enmies]);
	}
	deathAnimation(t_blocks);
	moveMent(t_player, t_blocks);

	return temp;
}

void Enemies::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		t_window.draw(bodies[i]);
		gun[i].render(t_window);
	}
}

int Enemies::hit(sf::RectangleShape t_player)
{
	bool temp = false;
	for (int i = 0; i < bodies.size(); i++)
	{
		if (gun[i].hit(t_player))
		{
			temp = true;
		}
	}
	return temp;
}

bool Enemies::getAlive(int t_index)
{
	return alive[t_index];
}

sf::RectangleShape Enemies::getBody(int t_index)
{
	return bodies[t_index];
}

int Enemies::getRealSize()
{
	return realMax;
}

void Enemies::moveMent(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		if (alive[i])
		{
			velo[i].x *= .9;
			velo[i].y += .98;
			//left and right movement
			if (t_player.getPosition().x + t_player.getSize().x < bodies[i].getPosition().x
				&& sqrt(pow(t_player.getPosition().x - bodies[i].getPosition().x, 2) + pow(t_player.getPosition().y - bodies[i].getPosition().y, 2)) < 1000)
			{
				velo[i].x -= MAX_MOVE;
			}
			else if (t_player.getPosition().x > bodies[i].getPosition().x + bodies[i].getSize().x
				&& sqrt(pow(t_player.getPosition().x - bodies[i].getPosition().x, 2) + pow(t_player.getPosition().y - bodies[i].getPosition().y, 2)) < 1000)
			{
				velo[i].x += MAX_MOVE;
			}
			//wall collision
			/*for (int index = 0; index < t_blocks.size(); index++)
			{
				if (t_blocks.at(index).getFillColor() == WALL_COLOUR && bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
				{
					if (velo[i].x > 0)
					{
						bodies[i].setPosition(t_blocks.at(index).getGlobalBounds().left - bodies[i].getSize().x, bodies[i].getPosition().y);
					}
					else
					{
						bodies[i].setPosition(t_blocks.at(index).getGlobalBounds().left + t_blocks.at(index).getGlobalBounds().width, bodies[i].getPosition().y);
					}
				}
			}*/

			for (int index = 0; index < t_blocks.size(); index++)
			{
				if (t_blocks.at(index).getFillColor() == FLOOR_COLOUR || (t_player.getPosition().y < bodies[i].getPosition().y + bodies[i].getSize().y))
				{
					if (bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()) && velo[i].y >= 0 &&
						bodies[i].getPosition().y + bodies[i].getSize().y - t_blocks.at(index).getPosition().y < velo[i].y)
					{
						bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x, t_blocks.at(index).getGlobalBounds().top - bodies[i].getSize().y));
						velo[i].y = 0;
					}
				}
			}
			for (int index = 0; index < bodies.size(); index++)
			{
				if (index != i)
				{
					//checks if enemy is dead
					if (!alive[index])
					{
						if (bodies[i].getGlobalBounds().intersects(bodies[index].getGlobalBounds()))  //if enemy collids with enemy corspe
						{
							if (velo[i].y >= 0)//if enemy moving down
							{
								if (bodies[index].getRotation() == 90)
								{
									if (bodies[i].getPosition().y + bodies[i].getSize().y - bodies[index].getPosition().y < velo[i].y)
									{
										bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x,bodies[index].getGlobalBounds().top - bodies[i].getSize().y));
										velo[i].y = 0;
									}
								}
								else if (bodies[index].getRotation() == 270)
								{
									if (bodies[i].getPosition().y + bodies[i].getSize().y -bodies[index].getPosition().y + bodies[index].getSize().x < velo[i].y)
									{
										bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x,
											bodies[index].getGlobalBounds().top - bodies[i].getSize().y));
										velo[i].y = 0;
									}
								}
							}

						}
					}
					
				}
			}

			if (t_player.getPosition().y + t_player.getSize().y < bodies[i].getPosition().y && velo[i].y == 0
				&& sqrt(pow(t_player.getPosition().x - bodies[i].getPosition().x, 2) + pow(t_player.getPosition().y - bodies[i].getPosition().y, 2)) < 1000)//jump 
			{
				velo[i].y = -bodies[i].getSize().y / 6;
			}
			
			bodies[i].setPosition(bodies[i].getPosition() + velo[i]);

			for (int index = 0; index < t_blocks.size(); index++)
			{
				if (t_blocks.at(index).getFillColor() == WALL_COLOUR)
				{
					if (bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
					{
						if (bodies[i].getPosition().x > t_blocks.at(index).getPosition().x && bodies[i].getPosition().x < t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x)
						{
							bodies[i].setPosition(t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x, bodies[i].getPosition().y);
						}
						else if(bodies[i].getPosition().x < t_blocks.at(index).getPosition().x && bodies[i].getPosition().x + bodies[i].getSize().x > t_blocks.at(index).getPosition().x )
						{
							bodies[i].setPosition(t_blocks.at(index).getPosition().x - bodies[i].getSize().x, bodies[i].getPosition().y);
						}
					}
				}
			}
			//slight spread
			for (int comp = 0; comp < realMax; comp++)
			{
				if (alive[comp] && i != comp)
				{
					if (bodies[i].getGlobalBounds().intersects(bodies[comp].getGlobalBounds()))
					{
						if (bodies[i].getPosition().x > bodies[comp].getPosition().x)
						{
							bodies[i].setPosition(bodies[i].getPosition().x + MAX_MOVE, bodies[i].getPosition().y);
						}
						else if (bodies[i].getPosition().x < bodies[comp].getPosition().x)
						{
							bodies[i].setPosition(bodies[i].getPosition().x - MAX_MOVE, bodies[i].getPosition().y);
						}
					}
				}
			}
		}
	}
}

void Enemies::deathAnimation(std::vector<sf::RectangleShape> t_blocks)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		if (!alive[i])
		{
			velo[i].x *= .9;
			velo[i].y += .98;
			bodies[i].setPosition(bodies[i].getPosition() + velo[i]);
			for (int index = 0; index < t_blocks.size(); index++)
			{
				
				if (t_blocks.at(index).getFillColor() == WALL_COLOUR && bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
				{
					if (velo[i].x > 0)
					{
						bodies[i].setPosition(t_blocks.at(index).getGlobalBounds().left - bodies[i].getSize().y, bodies[i].getPosition().y);
						if (bodies[i].getRotation() == 90)
						{
							bodies[i].move(bodies[i].getSize().y, 0);
						}
					}
					else
					{
						bodies[i].setPosition(t_blocks.at(index).getGlobalBounds().left + t_blocks.at(index).getGlobalBounds().width, bodies[i].getPosition().y);
						if (bodies[i].getRotation() == 90)
						{
							bodies[i].move(bodies[i].getSize().y, 0);
						}
					}
					break;
				}
				

				if (bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
				{
					if (toTheRight[i])
					{
						if (bodies[i].getRotation() != 90)
						{
							bodies[i].rotate(2);
						}
					}
					else
					{
						if (bodies[i].getRotation() != 270)
						{
							bodies[i].rotate(-2);
						}
					}
					while(bodies[i].getGlobalBounds().intersects( t_blocks.at(index).getGlobalBounds()))
					{
						bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x, bodies[i].getPosition().y - 1));
						velo[i].y = 0;
					}
					break;
				}
			}
			//corpse platforms
			for (int index = 0; index < bodies.size(); index++)
			{
				if (index != i)
				{
					if (bodies[i].getGlobalBounds().intersects(bodies.at(index).getGlobalBounds()))
					{
						if (toTheRight[i])
						{
							if (bodies[i].getRotation() != 90)
							{
								bodies[i].rotate(2);
							}
						}
						else
						{
							if (bodies[i].getRotation() != 270)
							{
								bodies[i].rotate(-2);
							}
						}
						while (bodies[i].getGlobalBounds().intersects(bodies.at(index).getGlobalBounds()) && !alive[index])
						{
							bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x, bodies[i].getPosition().y - 1));
							velo[i].y = 0;
						}
						break;
					}
				}
			}
		}
	}
}

