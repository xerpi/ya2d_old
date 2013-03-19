#include "ya2d_texture.h"

//Simple draw
	//Non swizzled
		//GU_SPRITES: 28.54 FPS
		//GU_TRIANGLE_STRIP: 37.36 FPS
	//Swizzled
		//GU_SPRITES: 89.91 FPS
		//GU_TRIANGLE_STRIP: 110.34 FPS
//Fast draw
	//Non swizzled
		//GU_SPRITES: 148.26 FPS
		//GU_TRIANGLE_STRIP: 142.57 FPS
	//Swizzled
		//GU_SPRITES: 264.74 FPS
		//GU_TRIANGLE_STRIP: 263.21 FPS


	void ya2d_createTexture(ya2d_Texture *texp, int w, int h, int psm)
	{
		texp->imageWidth  = w;
		texp->imageHeight = h;
		texp->textureWidth  = next_pow2(w);
		texp->textureHeight = next_pow2(h);
		texp->centerX = (int)(w/2);
		texp->centerY = (int)(h/2);

		texp->texPSM = psm;
		texp->isSwizzled = 0;
		texp->hasAlpha   = 1;
		
		switch(psm)
		{
		case GU_PSM_4444:
		case GU_PSM_5650:
		case GU_PSM_5551:
			texp->rowBytes = texp->textureWidth * 2;
			break;
		default:
		case GU_PSM_8888:
			texp->rowBytes = texp->textureWidth * 4;
			break;
		}
		
		texp->dataLength = texp->rowBytes * texp->textureHeight;
		texp->data = (uint8_t *)memalign(16, texp->dataLength);
		memset(texp->data, 0, texp->dataLength);
	}
	
	void ya2d_swizzleTexture(ya2d_Texture *texp)
	{
		if(texp->isSwizzled || (texp->textureWidth < 33 && texp->textureHeight < 33)) return;
		uint8_t *swizzledData = (uint8_t *)memalign(16, texp->dataLength);
		swizzle_fast(swizzledData, texp->data, texp->textureWidth * 4, texp->textureHeight);
		memcpy(texp->data, swizzledData, texp->dataLength);
		free(swizzledData);
		texp->isSwizzled = 1;
	}

    void ya2d_freeTexture(ya2d_Texture *texp)
    {
        if(texp->data != NULL)
        {
            free(texp->data);
            texp->data = NULL;
        }
    }

    void ya2d_drawTexture(ya2d_Texture *texp, int x, int y)
    {
        if(!texp->data) return;

		sceGuEnable(GU_TEXTURE_2D);
		sceGumPushMatrix();
		sceGumLoadIdentity();        

		if(texp->isSwizzled)
			sceGuTexMode(texp->texPSM, 0, 0, GU_TRUE);
		else
			sceGuTexMode(texp->texPSM, 0, 0, GU_FALSE);
					
		if(texp->hasAlpha)
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		else
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);

        sceGuTexImage(0, texp->textureWidth, texp->textureHeight, texp->textureWidth, texp->data);

		/*
        ya2d_TextureVertex *vertices = (ya2d_TextureVertex *)sceGuGetMemory(2 * sizeof(ya2d_TextureVertex));

        vertices[0] = (ya2d_TextureVertex){0, 0, x, y, 0};
        vertices[1] = (ya2d_TextureVertex){texp->textureWidth, texp->textureHeight, x+texp->textureWidth, y+texp->textureHeight, 0};

        sceGumDrawArray(GU_SPRITES, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
        
        sceKernelDcacheWritebackRange(vertices, 2 * sizeof(ya2d_TextureVertex));
        */

        /*
        ya2d_FloatTextureVertex *vertices = (ya2d_FloatTextureVertex *)sceGuGetMemory(4 * sizeof(ya2d_FloatTextureVertex));

        vertices[0] = (ya2d_FloatTextureVertex){0.0f, 0.0f, x, y, 0.0f};
        vertices[1] = (ya2d_FloatTextureVertex){1.0f, 0.0f, x+texp->textureWidth, y, 0.0f};
        vertices[2] = (ya2d_FloatTextureVertex){0.0f, 1.0f, x, y+texp->textureHeight, 0.0f};
        vertices[3] = (ya2d_FloatTextureVertex){1.0f, 1.0f, x+texp->textureWidth, y+texp->textureHeight, 0.0f};

        sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);
        
        sceKernelDcacheWritebackRange(vertices, 4 * sizeof(ya2d_FloatTextureVertex));
        */
       
        
        ya2d_TextureVertex *vertices = (ya2d_TextureVertex *)sceGuGetMemory(4 * sizeof(ya2d_TextureVertex));

        vertices[0] = (ya2d_TextureVertex){0, 0, x, y, 0};
        vertices[1] = (ya2d_TextureVertex){texp->textureWidth, 0, x+texp->textureWidth, y, 0};
        vertices[2] = (ya2d_TextureVertex){0, texp->textureHeight, x, y+texp->textureHeight, 0};
        vertices[3] = (ya2d_TextureVertex){texp->textureWidth, texp->textureHeight, x+texp->textureWidth, y+texp->textureHeight, 0};

        sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);
        
        sceKernelDcacheWritebackRange(vertices, 4 * sizeof(ya2d_TextureVertex));
        
        sceGumPopMatrix();
    }

    void ya2d_drawTextureFast(ya2d_Texture *texp, int x, int y)
    {
		if(texp->textureWidth <= YA2D_TEXTURE_SLICE) ya2d_drawTexture(texp, x, y);
        if(!texp->data) return;

		sceGuEnable(GU_TEXTURE_2D);
		sceGumPushMatrix();
		sceGumLoadIdentity();        

		if(texp->isSwizzled)
			sceGuTexMode(texp->texPSM, 0, 0, GU_TRUE);
		else
			sceGuTexMode(texp->texPSM, 0, 0, GU_FALSE);
					
		if(texp->hasAlpha)
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		else
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);

        sceGuTexImage(0, texp->textureWidth, texp->textureHeight, texp->textureWidth, texp->data);
        
		int i;
		for(i = 0; i < texp->textureWidth; i+= YA2D_TEXTURE_SLICE)
		{
			/*
			ya2d_TextureVertex *vertices = (ya2d_TextureVertex *)sceGuGetMemory(4 * sizeof(ya2d_TextureVertex));
			vertices[0] = (ya2d_TextureVertex){i, 0, x+i, y, 0};
			vertices[1] = (ya2d_TextureVertex){i+YA2D_TEXTURE_SLICE, 0, x+i+YA2D_TEXTURE_SLICE, y, 0};
			vertices[2] = (ya2d_TextureVertex){i, texp->textureHeight, x+i, y+texp->textureHeight, 0};
			vertices[3] = (ya2d_TextureVertex){i+YA2D_TEXTURE_SLICE, texp->textureHeight, x+i+YA2D_TEXTURE_SLICE, y+texp->textureHeight, 0};
			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);
			sceKernelDcacheWritebackRange(vertices, 4 * sizeof(ya2d_TextureVertex));
			*/
			
			ya2d_TextureVertex *vertices = (ya2d_TextureVertex *)sceGuGetMemory(2 * sizeof(ya2d_TextureVertex));
			vertices[0] = (ya2d_TextureVertex){i, 0, x+i, y, 0};
			vertices[1] = (ya2d_TextureVertex){i+YA2D_TEXTURE_SLICE, texp->textureHeight, x+i+YA2D_TEXTURE_SLICE, y+texp->textureHeight, 0};
			sceGumDrawArray(GU_SPRITES, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
			sceKernelDcacheWritebackRange(vertices, 2 * sizeof(ya2d_TextureVertex));
		}
			   
		sceGumPopMatrix();
	}

void ya2d_drawRotateTexture(ya2d_Texture *texp, int x, int y, float angle)
{
	/*sceGuDisable(GU_TEXTURE_2D);
		sceGumPushMatrix();
		sceGumLoadIdentity();
		ScePspFVector3 p = {(float)x, (float)y, 0.0f};
		
		ya2d_FloatVertex __attribute__((aligned(16))) vert[3] =
		{
			   {GU_RGB(255,0,0), 0.0f, -50.0f, 0.0f}, // Top, red
			   {GU_RGB(0,255,0), 50.0f, 50.0f, 0.0f}, // Right, green
			   {GU_RGB(0,0,255), -50.0f, 50.0f, 0.0f}, // Left, blue
		};
		sceGumTranslate(&p);
        sceGumRotateZ(angle);
        
        sceGumDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,vert);


		
		sceGumLoadIdentity();
		p.x += 51.0f;
		sceGumTranslate(&p);
        sceGumRotateZ(angle);
        
        sceGumDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,vert);

		sceGumPopMatrix();
		*/

}
