/*
*  collision.c
*
*  Created on: 29 Apr 2016
*      Author: jake
*/
#include "defs.h"

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	//if any two blocks collide, return 1
	return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

void collisionDetect(GameState *game)
{

	for(int i = 0; i < 8; i++)
	{
		//if the player collides with any of the sprites
		if(collide2d(game->player.x, game->player.y, game->plog[i].x, game->plog[i].y, 32, 32, 32, 32))
		{
			 //if the player isn't dead
			if(!game->player.isDead)
			{
				//make him dead and play the death music
				game->player.isDead = 1;
				Mix_HaltChannel(game->musicChannel);
				Mix_PlayChannel(-1, game->dieSound, 0);
			}
		}
	}

	//if the player falls out of bounds of the map
	if(game->player.y > game->map.maxY)
	{
		if(!game->player.isDead)
		{
			//make him dead and play the death musi
			game->player.isDead = 1;
			Mix_HaltChannel(game->musicChannel);
			Mix_PlayChannel(-1, game->dieSound, 0);
		}
	}

	//set parameters for width/height of boxes
	float mw = 25, mh = 48;
	float bw = 32, bh = 32;
	float mx = game->player.x, my = game->player.y;

	//Check for collision with any ledges (brick blocks)
	for(int i = 0; i < game->map.maxX; i++)
	{
		//set co-ordinates for ledge
		float bx = game->ledge[i].bx;
		float by = game->ledge[i].by;

		if(mx+mw/2 > bx && mx+mw/2<bx+bw)
		{
			//are we bumping our head?
			if(my < by+bh && my > by && game->player.dy < 0)
			{
				//correct y
				game->player.y = by+bh;
				my = by+bh;

				//bumped our head, stop any jump velocity
				game->player.dy = 0;
				game->player.jumpCount = 2;
			}
		}

		if(mx+mw > bx && mx<bx+bw)
		{
			//are we landing on the ledge?
			if(my+mh > by && my < by && game->player.dy > 0)
			{
				//correct y
				game->player.y = by-mh;
				my = by-mh;

				//landed on this ledge, stop any jump velocity
				game->player.dy = 0;
				game->player.onLedge = 1;
			}
		}

		if(my+mh > by && my<by+bh)
		{
			//rubbing against right edge
			if(mx < bx+bw && mx+mw > bx+bw && game->player.dx < 0)
			{
				//correct x
				game->player.x = bx+bw;
				game->player.dx = 0;
				game->player.walking = 0;
				game->player.currentSprite = 9;
			}

			//rubbing against left edge
			else if(mx+mw > bx && mx < bx && game->player.dx > 0)
			{
				//correct x
				game->player.x = bx-mw;
				game->player.dx = 0;
				game->player.walking = 0;
				game->player.currentSprite = 9;
			}
		}
	}
}

