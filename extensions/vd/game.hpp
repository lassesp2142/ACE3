#pragma once

#include "shared.hpp"
#include "vector.hpp"
#include "arguments.hpp"
#include "materials.hpp"

namespace ace {
    namespace vehicledamage {

        // Hitpoints need to be either generically defined, or defined in the model
        // We need to estimate hitpoint locations if they are not provided in the model, especially for advanced ACE
        // hitpoints. For now, lets assume they ahve them all defined in the config (instead of model), and we can read either location

        enum class DAMAGE_TYPE {
            VISUAL = -1,
            EXPLOSIVE = 3,
            SHRAPNEL = 4,
            DAMAGE = 5,
            DISABLE = 6,
            PRESSURE = 7,
            HEAT = 8,
            FIRE = 9
        };

        enum class HITPOINT_TYPE {
            ENGINE = 1,
            AMMO = 2,
            MAGAZINE = 3,
            CREW = 4,
            TRACK = 5,
            WHEEL = 6,
            WEAPON = 7,
            TURRET = 9,
            COMPUTER = 10,
            SIGHT = 11
        };

        class damage_entry {
        public:
            bool placeholder_1;
        };

        class hitpoint {
        public:
            HITPOINT_TYPE                    type;
            std::string                      name;

            std::unordered_map<std::string, arguments> parameters;

            std::vector<ace::vector3<float>> points;
        };
        typedef std::shared_ptr<hitpoint> hitpoint_p;

        typedef std::pair<damage_entry, hitpoint_p> damage_pair;

        enum class PROJECTILE_TYPE {
            CYLINDER = 1,
            FRUSTUM = 2,    // This is long rod, but working length changes
            MISSILE = 3,    // These will most likely be a hybrid of rod projected kenetic rounds
            BULLET = 4,     // Not used? Can we just use cylinder long rod equations?
            FRAGMENT = 5
        };

        class projectile {
        public:
            projectile(uint32_t projtypeid_, const std::string & classname_, float length_, float diameter_, float material_density_, int material_type_, float frustum_length_, float frustum_diameter_,
                ace::vector3<float> vel, ace::vector3<float> pos, ace::vector3<float> dir) :
                type(PROJECTILE_TYPE(projtypeid_)), classname(classname_), length(length_), diameter(diameter_), material_density(material_density_), material_type(material_type_), frustum_length(frustum_length_), frustum_diameter(frustum_diameter_),
                velocity(vel), position(pos), direction(dir)
            {}

            projectile & operator= (const projectile& other) {
                type = other.type; classname = other.classname; length = other.length; diameter = other.diameter, material_density = other.material_density; material_type = other.material_type;
                frustum_length = other.frustum_length; frustum_diameter = other.frustum_diameter; 
                velocity = other.velocity; position = other.position; direction = other.direction;
                return *this;
            }

            PROJECTILE_TYPE     type;          // bullet, longrod, shell, etc.

            std::string         classname;

            float               length;
            float               diameter;

            float               material_density;
            int                 material_type;

            float               frustum_diameter;
            float               frustum_length;

            std::unordered_map<std::string, float> extended;

            ace::vector3<float> velocity;
            ace::vector3<float> position;
            ace::vector3<float> direction;
        };

        // This class tracks incoming hits from the engine
        class gamehit {
        public:
            gamehit(uint32_t id, uint32_t objectid,
                const std::string & classname, const std::string & selection,
                ace::vector3<float> dir, ace::vector3<float> up,
                projectile proj,
                ace::vector3<float> orthosurface, ace::vector3<float> impactpos, ace::vector3<float> impactvel, std::string bisurf_, float material_density_, int material_type_)
                : id(id), objectid(objectid),
                classname(classname), selection(selection),
                vehicle_direction(dir), vehicle_up(up),
                projectile(proj),
                surface(orthosurface), impactposition(impactpos), impactvelocity(impactvel), bisurf(bisurf_),
                material_density(material_density_), material_type(material_type_)
            {}
#define PROJ_ARG_COUNT 16
            static std::shared_ptr<gamehit> create(const arguments & args) {
                return std::make_shared<gamehit>(-1, args.as_uint32(0), args.as_string(1), args.as_string(2),
                    args[3], args[4],
                    ace::vehicledamage::projectile(args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12],
                    args[13], args[14], args[15]),
                    args.as_vector(PROJ_ARG_COUNT), args.as_vector(PROJ_ARG_COUNT+1), args.as_vector(PROJ_ARG_COUNT+2), args[PROJ_ARG_COUNT+3],
                    args[PROJ_ARG_COUNT + 4], args[PROJ_ARG_COUNT + 5]);
            }
            
            uint32_t            id;
            uint32_t            objectid;

            std::string         classname;
            std::string         selection;

            projectile          projectile;

            ace::vector3<float> vehicle_direction;
            ace::vector3<float> vehicle_up;

            ace::vector3<float> surface;
            std::string         bisurf;

            ace::vector3<float> impactposition;
            ace::vector3<float> impactvelocity;

            int                 material_type;
            float               material_density;
        };
        typedef std::shared_ptr<gamehit> gamehit_p;
    }
}