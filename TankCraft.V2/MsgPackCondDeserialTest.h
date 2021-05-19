#pragma once
#include <msgpack.hpp>
/*
* 
* Test and example of serializig/deserializing a component with conditional fields
* Here, dirty/optional fields are marked with a bitfield within the component.
* 
* For a developer to change the way msgpack serializes/deserializes, all that is required is to override the convert and pack functions.
* 
* Link to examples: https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_adaptor#convert
*					https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_unpacker
* 
*/



namespace CondTest {
	void condDeserialTest();

	class player {
	private:

	public:
		std::string name;
		int x;
		int y;
		std::vector<player> neighbors;

		std::vector<bool> fields;

		player() {
			fields = std::vector<bool>(4);
			name = std::string("roee"); fields[0] = true;
			x = 5; fields[1] = true;
			y = 10; fields[2] = true;
			neighbors = std::vector<player>(); fields[3] = true;
		}
	//	player(std::string const& name_, int x_, int y_, std::vector<player> const& neighbors_) :name(name_), x(x_), y(y_), neighbors(neighbors_) {}


		~player() = default;

		std::string const& get_name() const { return name; }
		int get_x() const { return x; }
		int get_y() const { return y; }
		std::vector<player> const& get_neighbors() const {return neighbors;}

		// TODO: setters and getters all set the respective fields fields

		MSGPACK_DEFINE(fields, name, x, y, neighbors);
	};
}

namespace msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {

			// Pack with conditional field setter logic and bandwidth optimization
			// NOTE: fields must be packed in order of define statement, causes undefined behaviour otherwise
			template<>
			struct pack<CondTest::player> {
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, CondTest::player const& v) const {
					// packing member variables as an array.
					int fields_set = 1;
					for (int i = 0; i < 4; i++)
						if (v.fields[i]) fields_set++;
					o.pack_array(fields_set);
					o.pack(v.fields);

					if (v.fields[0]) {
						o.pack(v.get_name());
					}
					if (v.fields[1]) {
						o.pack(v.get_x());
					}
					if (v.fields[2]) {
						o.pack(v.get_y());
					}
					if (v.fields[3]) {
						o.pack(v.get_neighbors());
					}
					return o;
				}
			};
			/*
			template <>
			struct object_with_zone<CondTest::player> {
				void operator()(msgpack::object::with_zone& o, CondTest::player const& v) const {
					o.type = type::ARRAY;
					int fields_set = 0;
					for (int i = 0; i < 5)
					o.via.array.size = v.;
					o.via.array.ptr = static_cast<msgpack::object*>(
						o.zone.allocate_align(sizeof(msgpack::object) * o.via.array.size));
					o.via.array.ptr[0] = msgpack::object(v.get_name(), o.zone);
					o.via.array.ptr[1] = msgpack::object(v.get_age(), o.zone);
				}
			};
			*/


			template <>
			struct convert<CondTest::player> {
				// Convert with conditional field setter logic and bandwidth optimization
				// NOTE: assumes that fields are packed in order of the define statement, undefined behaviour otherwise
				msgpack::object const& operator()(msgpack::object const& o, CondTest::player& v) const {
					if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
				//	if (o.via.array.size != 5) throw msgpack::type_error();
					// Always do this line, so we have access to the fields

					o.via.array.ptr[0] >> v.fields;
					int head = 1;

					if (v.fields[0]) {
						o.via.array.ptr[head] >> v.name;
						head++;
					}
					if (v.fields[1]) {
						o.via.array.ptr[head] >> v.x;
						head++;
					}
					if (v.fields[2]) {
						o.via.array.ptr[head] >> v.y;
						head++;
					}
					if (v.fields[3]) {
						o.via.array.ptr[head] >> v.neighbors;
					}
					return o;
				}
			};
		}
	}
}
//neighbors.push_back(player()); neighbors.push_back(player());