LCD-OLinuXino-4.3TS is a 4.3 inch display with resolution of 480x272 and resistive touch screen.

LCD-OLINUXINO-4.3 and LCD-OLINUXINO-4.3TS use the same design files (schematic, board files).

Both boards use the legacy driver board design, not the common LCD-DRIVER board.

Hardware revision changes:

Hardware revision A
========
Initial release
	
Hardware revision B
========
1. Changed the package of L1 from SD75 to L7078(YS75).
2. Re-routed 28V wire, moved further from L1.
3. Added https prefix on the white print of the link to the web-site.

Hardware revision C
========
1. HSYNC and VSYNC were added, because they are required for some batches of new 4.3" LCDs.

Hardware revision D
========
Changes in this revision should not affect the user, these are purely manufacturing optimizations!
1. Changed LCD connector package for easier soldering;
2. Changed LIME and LIME2 connectors to ones with 0.9mm holes.
