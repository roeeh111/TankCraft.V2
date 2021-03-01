#pragma once
#define MAX_ENTITIES 0xFF

namespace FreeListComponent {
	// This is the freemap component for the idtranslation system
	// It is not to be networked, and is only to be used for the server
	// THIS IS A BIG MUTHERFUCKER, DONT LET ANY CLIENT OR ENTITY USE IT!!!!!
	typedef struct freelist_ {
		bool map[MAX_ENTITIES];
		uint32_t lastMapping;
		// constructor to zero out everything, by god and for all that is holy do not do this more than once
		freelist_() {
			for (int i = 0; i < MAX_ENTITIES; i++) {
				map[i] = false;
			}
			lastMapping = 0;
		}
	} freelist;
}