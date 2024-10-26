#include "view.h"
#include "controller.h"

int main()
{
	clear_terminal();	// Start wiht clean terminal
	print_hellowep();	// Just fun ascii :) wep = wonder egg priority (an anime)
	boot_magic_chess(); // The game loop
	return 0;
}