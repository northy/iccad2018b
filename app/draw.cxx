#include <parser.hpp>
#include <draw.hpp>

#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>

#include <sstream>
#include <cmath>
#include <string>
#include <iostream>

using namespace ast;

using std::cout, std::cin, std::cerr;
using std::vector, std::pair, std::string;

int main(int narg, char** argv)
{
#ifdef CAIRO_HAS_SVG_SURFACE
    auto result = parser::parse_file(argv[1]);
    if(result) {
        string filename = "image.svg";
        int64_t width = result->boundary.p2.x;
        int64_t height = result->boundary.p2.y;
        Cairo::RefPtr<Cairo::SvgSurface> surface =
            Cairo::SvgSurface::create(filename, width, height);

        Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

        Draw draw = Draw(cr);

        cr->paint(); // fill image with the color

        cr->set_source_rgb(255, 215, 0);
        for (const Bus & bus:result->buses) {
            for (const Bit & bit:bus.bits) {
                for (const RoutedShape & pin:bit.shapes) {
                    draw.routedShape(pin);
                }
            }
        }

        for (const Track & t:result->tracks) {
            draw.track(t);
        }

        cr->save();
        cr->show_page();

        std::cout << "Wrote SVG file \"" << filename << "\"" << std::endl;
        return 0;
    }
    else {
        cerr << "Parser error." << std::endl;
        return -1;
    }

#else
    std::cout << "You must compile cairo with SVG support for this application to work."
        << std::endl;
    return 1;

#endif
}

/*
cr->save();
        // draw a border around the image
        cr->set_line_width(20.0); // make the line wider
        cr->rectangle(0.0, 0.0, width, height);
        cr->stroke();

        cr->set_source_rgba(0.0, 0.0, 0.0, 0.7);
        // draw a circle in the center of the image
        cr->arc(width / 2.0, height / 2.0,             height / 4.0, 0.0, 2.0 * M_PI);
        cr->stroke();

        // draw a diagonal line
        cr->move_to(width / 4.0, height / 4.0);
        cr->line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
        cr->stroke();
        cr->restore();
*/