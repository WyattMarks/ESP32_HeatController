module place_mount_hole(diameter) {
    linear_extrude(10, convexity=6) {
        $fn = 24;
        circle(r=diameter/2);
    }
}

module box(size, wallWidth) {
    difference() {
        cube(size);
        translate( [wallWidth, wallWidth, wallWidth] )
        cube( [size[0] - 2*wallWidth, size[1] - 2*wallWidth, size[2] - 2*wallWidth] );
    }
}



difference() {
    union() {
        //Relay+Iron Mounting
        difference() {
        
            union() {
                
                //Top piece
                difference() {
                    cube([80,102,10]);
                    translate( [(80-44) / 2, 10, 0] )
                    place_mount_hole(4);
                    translate( [(80-44) / 2 + 44, 10, 0] )
                    place_mount_hole(4);
                }
                
                //Side walls
                translate( [0, 0, -40] )
                cube( [ (80-48) / 2, 102, 40] );
                translate( [ (80-48) / 2 + 48, 0, -40] )
                cube( [ (80-48) / 2, 102, 40] );
                translate( [0, 102, -40-48-8] )
                cube( [80, 20, 50+48+8] );
                translate( [0, 0, -88-8] )
                cube( [10, 40, 48+8] );
                translate( [70, 0, -88-8] )
                cube( [10, 40, 48+8] );
                
                //Inside wall
                translate( [ 0, 30, -40-48] )
                cube( [80, 10, 40+48] );
                

            }
            
            //Access to relay
            translate( [(80-48) / 2, 30, -40] )
            cube( [48, 10, 40] );
            
            //Thinner walls so mount holes can be reached
            translate( [10, 0, -40] )
            cube( [ 80-20, 30, 40] );
            
            //Relay mount holes
            translate( [40, 36, -88] )
            place_mount_hole(4.85);
            translate( [40, 36+70, -88] )
            place_mount_hole(4.85);
            translate( [10, 102, -88-8] )
            cube( [60, 10, 8] );
            
            //AC Strain Relief
            translate( [10, 102, -88-8] )
            cube( [23, 20, 8] );
            translate( [10+12.25, 102+12, -88-8+2] )
            place_mount_hole(4.5);
        }


        //Front housing
        
        translate( [0, -68, -88-8] )
        cube( [10, 68, 88+8+10] );
        
        translate( [70, -68, -88-8] )
        cube( [10, 68, 88+8+10] );
        
        translate( [0, -68, -88-8] )
        cube( [80, 10, 88+8+10] );
        
        translate( [50, -68+52+10, -88-8] )
        cube( [20, 6, 88+8+10] );

        
        
    }
    
    //5V Supply mounting
    translate( [ 80-10-15.8, -68+52+10, -88-8+11.5] )
    rotate( [-90, 0, 0] )
    place_mount_hole(3);
    translate( [ 80-10-15.8, -68+52+10, -88-8+11.5+39] )
    rotate( [-90, 0, 0] )
    place_mount_hole(3);
    
    
    //Thermistor wire exit
    translate( [ 0, -15, -30] )
    rotate( [0, 90, 0] )
    place_mount_hole(3);
}
