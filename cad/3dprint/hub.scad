/* SETTINGS*/
$fn=100;
hstep1 = 3; // base height
hstep2 = 3; // protrusion height
id = 6.34; // inner dia (encoder shaft dia)
od = 18; // outer dia (laser cutting cutout)
radius = 25; // base radius
hradius = 38; // radius about which mouting holes sit
screw = 3; // mouting screw size
intf = 0.2; // interference fit reduction
// keyway
keyway = true; // add keyway
keywidth = 2.5; // keyway width
keydepth = 1; // keyway depth
/*=============================*/

/* WORKING*/
iid = id - intf; // tolerance fit inner dia

union() {
  base();
  translate([0,0,hstep1]) shaft();
}

module base() {
  difference() {
    cylinder(r=radius, h=hstep1);
    cylinder(r=iid/2, h=hstep1);
    if (keyway) {
      translate([iid/2-keydepth/2,-keywidth/2,0]) cube([keydepth,keywidth,hstep1]);
    }
    holes(hradius);
    rotate(90) holes(hradius);
  }
}

module holes(radius) {
  translate([-radius/2,0,0]) cylinder(r=screw/2,h=hstep1);
  translate([radius/2,0,0]) cylinder(r=screw/2,h=hstep1);
}

module shaft() {
  difference() {
    cylinder(r=od/2,h=hstep2);
    cylinder(r=iid/2,h=hstep2);
    if (keyway) {
      translate([iid/2-keydepth/2,-keywidth/2,0]) cube([keydepth,keywidth,hstep2]);
    }
  }
}
