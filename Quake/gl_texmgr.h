/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2010-2014 QuakeSpasm developers
Copyright (C) 2016 Axel Gneiting

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _GL_TEXMAN_H
#define _GL_TEXMAN_H

//gl_texmgr.h -- fitzquake's texture manager. manages opengl texture images

#define TEXPREF_NONE			0x0000
#define TEXPREF_MIPMAP			0x0001	// generate mipmaps
// TEXPREF_NEAREST and TEXPREF_LINEAR aren't supposed to be ORed with TEX_MIPMAP
#define TEXPREF_LINEAR			0x0002	// force linear
#define TEXPREF_NEAREST			0x0004	// force nearest
#define TEXPREF_ALPHA			0x0008	// allow alpha
#define TEXPREF_PAD			0x0010	// allow padding
#define TEXPREF_PERSIST			0x0020	// never free
#define TEXPREF_OVERWRITE		0x0040	// overwrite existing same-name texture
#define TEXPREF_NOPICMIP		0x0080	// always load full-sized
#define TEXPREF_FULLBRIGHT		0x0100	// use fullbright mask palette
#define TEXPREF_NOBRIGHT		0x0200	// use nobright mask palette
#define TEXPREF_CONCHARS		0x0400	// use conchars palette
#define TEXPREF_WARPIMAGE		0x0800	// resize this texture when warpimagesize changes

enum srcformat {SRC_INDEXED, SRC_LIGHTMAP, SRC_RGBA};

typedef uintptr_t src_offset_t;
struct glheap_s;
struct glheapnode_s;

typedef struct gltexture_s {
//managed by texture manager
	struct gltexture_s	*next;
	qmodel_t		*owner;
//managed by image loading
	char			name[64];
	unsigned int		width; //size of image as it exists in opengl
	unsigned int		height; //size of image as it exists in opengl
	unsigned int		flags;
	char			source_file[MAX_QPATH]; //relative filepath to data source, or "" if source is in memory
	src_offset_t		source_offset; //byte offset into file, or memory address
	enum srcformat		source_format; //format of pixel data (indexed, lightmap, or rgba)
	unsigned int		source_width; //size of image in source data
	unsigned int		source_height; //size of image in source data
	unsigned short		source_crc; //generated by source data before modifications
	signed char			shirt; //0-13 shirt color, or -1 if never colormapped
	signed char			pants; //0-13 pants color, or -1 if never colormapped
//used for rendering
	VkImage				image;
	VkImageView			image_view;
	VkImageView			target_image_view;
	struct glheap_s *	heap;
	struct glheapnode_s *	heap_node;
	VkDescriptorSet		descriptor_set;
	VkFramebuffer		frame_buffer;
	VkDescriptorSet		warp_write_descriptor_set;
	int			visframe; //matches r_framecount if texture was bound this frame
} gltexture_t;

extern gltexture_t *notexture;
extern gltexture_t *nulltexture;
extern gltexture_t *whitetexture;
extern gltexture_t *greytexture;

extern unsigned int d_8to24table[256];
extern unsigned int d_8to24table_fbright[256];
extern unsigned int d_8to24table_nobright[256];
extern unsigned int d_8to24table_conchars[256];
extern unsigned int d_8to24table_shirt[256];
extern unsigned int d_8to24table_pants[256];

// TEXTURE MANAGER

float TexMgr_FrameUsage (void);
gltexture_t *TexMgr_FindTexture (qmodel_t *owner, const char *name);
gltexture_t *TexMgr_NewTexture (void);
void TexMgr_FreeTexture (gltexture_t *kill);
void TexMgr_FreeTextures (unsigned int flags, unsigned int mask);
void TexMgr_FreeTexturesForOwner (qmodel_t *owner);
void TexMgr_NewGame (void);
void TexMgr_Init (void);
void TexMgr_DeleteTextureObjects (void);
void TexMgr_CollectGarbage (void);

// IMAGE LOADING
gltexture_t *TexMgr_LoadImage (qmodel_t *owner, const char *name, int width, int height, enum srcformat format,
			       byte *data, const char *source_file, src_offset_t source_offset, unsigned flags);
void TexMgr_ReloadImage (gltexture_t *glt, int shirt, int pants);
void TexMgr_ReloadNobrightImages (void);

int TexMgr_Pad(int s);
int TexMgr_SafeTextureSize (int s);
int TexMgr_PadConditional (int s);

void TexMgr_UpdateTextureDescriptorSets(void);


#endif	/* _GL_TEXMAN_H */

