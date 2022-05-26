OUTPUT(tcpgecko.elf);

/* Tell linker where our application entry is so the garbage collect can work correct */
ENTRY(__entry_menu);

SECTIONS {
	. = 0x00802000;
	.text : {
		*(.kernel_code*);
		*(.text*);
		/* Tell linker to not garbage collect this section as it is not referenced anywhere */
		KEEP(*(.kernel_code*));
	}
	.rodata : {
		*(.rodata*);
	}
	.data : {
		*(.data*);

		__sdata_start = .;
		*(.sdata*);
		__sdata_end = .;

		__sdata2_start = .;
		*(.sdata2*);
		__sdata2_end = .;
	}
	.bss : {
		__bss_start = .;
		*(.bss*);
		*(.sbss*);
		*(COMMON);
		__bss_end = .;
	}
	__CODE_END = .;

	/DISCARD/ : {
		*(*);
	}



		/* misc. */
		__nw__FUi = 0x0382AACC;
		__dl__FPv = 0x382ABB4;

		/* Item */
		releaseUsing__4ItemF35not_null_ptr__tm__15_12ItemInstanceP5LevelQ2_5boost33shared_ptr__tm__15_12LivingEntityi = 0x024d00ec;
		registerItem__4ItemSFiRC16ResourceLocationP4Item = 0x024b3d60;
		getId__4ItemSFP4Item = 0x02492100;
		byId__4ItemSFi = 0x02486698;

		/* BowItem */
		__vtbl__7BowItem = 0x10563754;
		__ct__7BowItemFv = 0x02162460;
		releaseUsing__7BowItemF35not_null_ptr__tm__15_12ItemInstanceP5LevelQ2_5boost33shared_ptr__tm__15_12LivingEntityi = 0x02164810;

		/* BufferBuilder */
		begin__13BufferBuilderFi = 0x02FD1FB4;
		color__13BufferBuilderFi = 0x02FD36A8;
		color__13BufferBuilderFiN21 = 0x02FD20B4;
		color__13BufferBuilderFiN31 = 0x02FD2020;
		color__13BufferBuilderFiT1 = 0x02FD36C4;
		end__13BufferBuilderFv = 0x02FD2288;
		vertex__13BufferBuilderFfN21 = 0x02FD2A34;

		/* Font */
		bindTexture__4FontFv = 0x03128004;
		cacheCharacter__4FontFi = 0x031260A8;
		CharacterExists__4FontFw = 0x030E7874;
		drawNoBeginEndBuilder__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN22 = 0x03127FEC;
		drawShadow__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN22 = 0x03126B88;
		drawWordWrapInternal__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN32bT2 = 0x03127788;
		drawWordWrapInternal__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN42 = 0x03127770;
		drawWordWrap__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN42 = 0x0312801C;
		draw__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wbT2 = 0x030E9534;
		draw__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN22 = 0x030E9B84;
		draw__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wiN22bT5 = 0x030E9A10;
		MapCharacter__4FontFw = 0x030E8A60;
		renderCharacter__4FontFwP13BufferBuilder = 0x030E93AC;
		sanitize__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wb = 0x030E8D60;
		width__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_w = 0x030E919C;
		wordWrapHeight__4FontFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wi = 0x03127048;
		__ct__4FontFP7OptionsRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wP8TexturesbP16ResourceLocationiN36PUs = 0x031261EC;

		/* GlStateManager */
		__CPR59__disableTexGen__14GlStateManagerSFQ2_J15J6TexGen = 0x03124DD4;
		__CPR72__texGenParam__14GlStateManagerSFQ2_J13J6TexGeniP11FloatBuffer = 0x03124E5C;
		activeTexture__14GlStateManagerSFi = 0x03124EEC;
		alphaFunc__14GlStateManagerSFif = 0x031056D8;
		bindTexture__14GlStateManagerSFi = 0x030E4750;
		blendFuncSeparate__14GlStateManagerSFiN31 = 0x0310B478;
		blendFunc__14GlStateManagerSFiT1 = 0x030E52A0;
		callList__14GlStateManagerSFi = 0x031252B8;
		callLists__14GlStateManagerSFP9IntBuffer = 0x03125338;
		clearColor__14GlStateManagerSFfN31 = 0x03105ECC;
		clearDepth__14GlStateManagerSFd = 0x03125214;
		clear__14GlStateManagerSFi = 0x031078C8;
		color3f__14GlStateManagerSFfN21 = 0x030E9394;
		color4f__14GlStateManagerSFfN31 = 0x030E4BA8;
		colorMask__14GlStateManagerSFbN31 = 0x03105704;
		colorMaterial__14GlStateManagerSFiT1 = 0x031082D8;
		cullFace__14GlStateManagerSFi = 0x0310B2BC;
		deleteLists__14GlStateManagerSFiT1 = 0x03125314;
		deleteTexture__14GlStateManagerSFi = 0x03124F10;
		deleteTextures__14GlStateManagerSFP9IntBuffer = 0x03125018;
		depthFunc__14GlStateManagerSFi = 0x030E9BD4;
		depthMask__14GlStateManagerSFb = 0x030E52FC;
		disableAlphaTest__14GlStateManagerSFv = 0x03108E10;
		disableBlend__14GlStateManagerSFv = 0x030E5328;
		disableClientState__14GlStateManagerSFi = 0x03125788;
		disableColorMaterial__14GlStateManagerSFv = 0x03116AB8;
		disableCull__14GlStateManagerSFv = 0x030F974C;
		disableDepthTest__14GlStateManagerSFv = 0x030E9B9C;
		disableFog__14GlStateManagerSFv = 0x03108C44;
		disableLight__14GlStateManagerSFi = 0x03124C04;
		disableLighting__14GlStateManagerSFv = 0x030E4B24;
		disableNormalize__14GlStateManagerSFv = 0x031251E4;
		disablePolygonOffset__14GlStateManagerSFv = 0x03124D20;
		disableRescaleNormal__14GlStateManagerSFv = 0x03111FB8;
		disableState__14GlStateManagerSFi = 0x030E49B0;
		disableTexture__14GlStateManagerSFv = 0x030E6268;
		enableAlphaTest__14GlStateManagerSFv = 0x031056BC;
		enableBlend__14GlStateManagerSFv = 0x030E5284;
		enableColorMaterial__14GlStateManagerSFv = 0x031082BC;
		enableCull__14GlStateManagerSFv = 0x030F9768;
		enableDepthTest__14GlStateManagerSFv = 0x030E9BB8;
		enableFog__14GlStateManagerSFv = 0x03108BA4;
		enableLight__14GlStateManagerSFi = 0x03124BE0;
		enableLighting__14GlStateManagerSFv = 0x030E4C04;
		enableNormalize__14GlStateManagerSFv = 0x031251CC;
		enablePolygonOffset__14GlStateManagerSFv = 0x03124D04;
		enableRescaleNormal__14GlStateManagerSFv = 0x0310B1A8;
		enableState__14GlStateManagerSFi = 0x030E489C;
		enableTexGen__14GlStateManagerSFQ2_14GlStateManager6TexGen = 0x03124D94;
		enableTexture__14GlStateManagerSFv = 0x030E6294;
		endList__14GlStateManagerSFi = 0x031252FC;
		flush__14GlStateManagerSFv = 0x03125784;
		fogDensity__14GlStateManagerSFf = 0x03105EA8;
		fogEnd__14GlStateManagerSFf = 0x03105E84;
		fogMode__14GlStateManagerSFi = 0x03108294;
		fogStart__14GlStateManagerSFf = 0x03105E60;
		genLists__14GlStateManagerSFi = 0x031253DC;
		genTexture__14GlStateManagerSFv = 0x03124EF8;
		genTextures__14GlStateManagerSFP9IntBuffer = 0x031250B4;
		getFogEnd__14GlStateManagerSFv = 0x03124CF4;
		getFogStart__14GlStateManagerSFv = 0x03124CE4;
		getMatrix__14GlStateManagerSFiP11FloatBuffer = 0x031078E4;
		getTexGen__14GlStateManagerSFQ2_14GlStateManager6TexGen = 0x03124D3C;
		glColor3f__14GlStateManagerSFfN21 = 0x0312578C;
		glFog__14GlStateManagerSFiP11FloatBuffer = 0x031080C4;
		glMultMatrixf__14GlStateManagerSFPf = 0x031257AC;
		lightModel__14GlStateManagerSFiP11FloatBuffer = 0x03124CB4;
		light__14GlStateManagerSFiT1P11FloatBuffer = 0x03124C28;
		lineWidth__14GlStateManagerSFf = 0x030E9BFC;
		loadIdentity__14GlStateManagerSFv = 0x03103080;
		matrixMode__14GlStateManagerSFi = 0x03103064;
		multMatrix__14GlStateManagerSFP11FloatBuffer = 0x0312527C;
		multMatrix__14GlStateManagerSFP8Matrix4f = 0x0312529C;
		multiTexCoord2f__14GlStateManagerSFfT1 = 0x030F9704;
		newList__14GlStateManagerSFiT1 = 0x031252DC;
		normal3f__14GlStateManagerSFfN21 = 0x030E9390;
		ortho__14GlStateManagerSFdN51 = 0x03110D4C;
		perspective__14GlStateManagerSFfN31 = 0x031030BC;
		polygonOffset__14GlStateManagerSFfT1 = 0x030E52D4;
		popMatrix__14GlStateManagerSFv = 0x030E4BD8;
		pushMatrix__14GlStateManagerSFv = 0x030E4B3C;
		rotated__14GlStateManagerSFdN31 = 0x03125224;
		rotatef__14GlStateManagerSFfN31 = 0x030E4B84;
		scaled__14GlStateManagerSFdN21 = 0x03103098;
		scalef__14GlStateManagerSFfN21 = 0x030E4B6C;
		setProfile__14GlStateManagerSFv = 0x031257C8;
		setupSolidRenderingTextureCombine__14GlStateManagerSFi = 0x03116AB0;
		shadeModel__14GlStateManagerSFi = 0x031251FC;
		staticCtor__14GlStateManagerSFv = 0x031254C0;
		tearDownSolidRenderingTextureCombine__14GlStateManagerSFv = 0x03116AB4;
		texGenMode__14GlStateManagerSFQ2_14GlStateManager6TexGeni = 0x03124E14;
		texImage2D__14GlStateManagerSFiN71P10ByteBuffer = 0x03125158;
		texLayerColor3f__14GlStateManagerSFifN22 = 0x0311ECB0;
		texParameteri__14GlStateManagerSFiN21 = 0x03103890;
		texParameteri__14GlStateManagerSFiT1 = 0x031216D4;
		translated__14GlStateManagerSFdN21 = 0x03125258;
		translatef__14GlStateManagerSFfN21 = 0x030E4B54;
		unsetProfile__14GlStateManagerSFv = 0x031257CC;
		viewport__14GlStateManagerSFiN31 = 0x031056B8;

		/* Minecraft */
		GetInstance__9MinecraftSFv = 0x03166818;
		getLevel__9MinecraftFi = 0x0318C878;

		/* ResourceLocation */
		__ct__16ResourceLocationFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wT1 = 0x03250D30;

		/* std::wstring */
		__ct__Q2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wFPCZ1Z = 0x020B08D4;

		/* Tesselator */
		getBuilder__10TesselatorFv = 0x03337EE8;
		getInstance__10TesselatorSFv = 0x03337EF0;

		/* ConsoleUIController */
		_ZN2mc18ui_controller_mainE = 0x109F95E0;

		/* SoundEvent */
		_ZN2mc10SoundEvent7note_bdE = 0x109C65A0;
		_ZN2mc10SoundEvent9note_harpE = 0x109C65A4;
		_ZN2mc10SoundEvent10note_plingE = 0x109C65AC;
		_ZN2mc10SoundEvent21random_successful_hitE = 0x109C61AC;

		/* Direction */
		_ZN2mc9Direction4downE = 0x109C46A0;
		_ZN2mc9Direction2upE = 0x109C46A4;


}

/******************************************************** FS ********************************************************/
/* coreinit.rpl difference in addresses 0xFE3C00 */