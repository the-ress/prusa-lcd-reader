//mirror([0, 0, 1]) // B
stencil();

module edge(){
    import("Edge_Cuts.gbr_svg.svg");
}

module paste(){
    difference(){
        import("F_Paste.gbr_iso_joined_svg.svg");
        edge();
    }
}

module pins(){
    difference(){
        import("drl_edit_conv_joined_svg.svg");
        edge();
    }
}

module stencil(){
    union(){
        difference(){
            hull(){
                linear_extrude(height = 0.15) {
                    offset(5) edge();
                }
            }
            linear_extrude(height = 1, center = true) {
                    paste();
            }
        }
        translate([0, 0, -.45]){
            linear_extrude(height = .45) {
                pins();
            }
        }
//        translate([0, 0, 0.125 + 0.10]){
//            hull(){
//                linear_extrude(height = 0.2) {
//                    edge();
//                }
//            }
//        }
    }
}
