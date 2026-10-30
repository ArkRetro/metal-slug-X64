/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x051df2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_51df2(void)

{
  code *pcVar1;
  int iVar2;
  char *pcVar3;
  char cVar4;
  ushort uVar5;
  char cVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined2 uStack_6;
  undefined4 uStack_4;

  uStack_4 = 0x51e00;
  candidate_1ad6e();
  uStack_4 = 0x51e08;
  candidate_50fb4();
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0x50f40;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 1;
  *unaff_A6 = 0x51e1c;
  uStack_4 = 0x51e22;
  candidate_1abc8();
  cVar6 = *(char *)((int)unaff_A6 + 0x9a) + -1;
  *(char *)((int)unaff_A6 + 0x9a) = cVar6;
  if (cVar6 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x9a) = 2;
    pcVar3 = *(char **)((int)unaff_A6 + 0x76);
    cVar6 = *pcVar3;
    cVar4 = pcVar3[1];
    if (cVar4 < '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x9a) = 0xf;
      *unaff_A6 = 0x51e7e;
      uStack_4 = 0x51e84;
      candidate_1abc8();
      cVar6 = *(char *)((int)unaff_A6 + 0x9a) + -1;
      *(char *)((int)unaff_A6 + 0x9a) = cVar6;
      if (cVar6 != '\0') {
        return;
      }
      uStack_4 = 0x51e96;
      candidate_50fb4();
      *(undefined4 *)((int)unaff_A6 + 0x76) = 0x50f48;
      *(undefined1 *)((int)unaff_A6 + 0x9a) = 0xff;
      *unaff_A6 = 0x51eaa;
      cVar6 = *(char *)(*(int *)((int)unaff_A6 + 0x76) + 1);
      if (cVar6 < '\0') {
        *unaff_A6 = 0x51f10;
      }
      else {
        *(int *)((int)unaff_A6 + 0x76) = *(int *)((int)unaff_A6 + 0x76) + 2;
        uStack_4 = CONCAT22((short)cVar6,uStack_4._2_2_);
        candidate_40bc();
        register0x0000003c = (BADSPACEBASE *)&uStack_4;
        candidate_1acf2();
        *(undefined2 *)(extraout_A0_00 + 0x1e) = *(undefined2 *)((int)unaff_A6 + 0x1e);
        *(undefined2 *)(extraout_A0_00 + 0x66) = *(undefined2 *)((int)unaff_A6 + 0x66);
        candidate_120f8();
        if (*(char *)((int)unaff_A6 + 0x9a) == '\0') {
          *(undefined1 *)(unaff_A6[3] + 0xa1) = *(undefined1 *)(unaff_A6[3] + 0x1a);
        }
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0x51f16;
      candidate_1abc8();
      *(char *)((int)unaff_A6 + 0x9a) = *(char *)((int)unaff_A6 + 0x9a) + '\x01';
      if (*(char *)((int)unaff_A6 + 0x9a) == '\0') {
        *(short *)(unaff_A6[3] + 0x2c) = *(short *)(unaff_A6[3] + 0x2c) + -6;
      }
      if (*(char *)((int)unaff_A6 + 0x9a) == '\x01') {
        *(short *)(unaff_A6[3] + 0x2c) = *(short *)(unaff_A6[3] + 0x2c) + -2;
      }
      if (*(char *)((int)unaff_A6 + 0x9a) == '\x02') {
        *(short *)(unaff_A6[3] + 0x2c) = *(short *)(unaff_A6[3] + 0x2c) + 4;
      }
      if (*(char *)((int)unaff_A6 + 0x9a) == '\x03') {
        *(undefined1 *)(unaff_A6[3] + 0x99) = 0xff;
        uVar8 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
        if (0xfc10 < uVar8) {
          uVar8 = *(ushort *)(unaff_A5 + 0x700a);
          *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
          uVar7 = uRam002fffe8;
          uVar5 = ~uVar8 & 0xfff;
          uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                           -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar5 & 7)) != 0));
          uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
          uRam002fffe2 = 0;
          uRam002fffe4 = 0;
          uRam002fffe6 = 0;
          *(short *)(unaff_A5 + 0x700a) =
               ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU)
               + *(short *)(unaff_A5 + 0x700a);
          if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar7) & 1) != 0) {
            *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
            _DAT_002ffff0 = 1;
            do {
            } while (DAT_002fffff != '\x01');
            uRam00400000 = 0;
            uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
            uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
            uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
            uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
            uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
            uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
            uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
            uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
            uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
            uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
            uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
            uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
            uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
            uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
            uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
            _DAT_00401ffe = 0;
            *(undefined4 *)((int)register0x0000003c + -4) = 0x3d2c;
            func_0x0003cf12();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x3d36;
            candidate_3e2a8();
            do {
                    /* WARNING: Do nothing block with infinite loop */
            } while( true );
          }
        }
        *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
        candidate_4126();
        iVar2 = *(int *)(unaff_A5 + -0x1c24);
        do {
          unaff_A6 = (undefined4 *)unaff_A6[2];
          pcVar1 = (code *)*unaff_A6;
          *(undefined4 *)(iVar2 + -4) = 0x424a;
          (*pcVar1)();
        } while( true );
      }
      return;
    }
    *(char **)((int)unaff_A6 + 0x76) = pcVar3 + 2;
    uStack_4 = CONCAT22((short)cVar4,uStack_4._2_2_);
    candidate_40bc();
    *(short *)(extraout_A0 + 0x92) = (short)cVar6;
    *(undefined2 *)(extraout_A0 + 0x94) = uStack_6;
    *(undefined2 *)(extraout_A0 + 0x1e) = *(undefined2 *)((int)unaff_A6 + 0x1e);
    *(undefined2 *)(extraout_A0 + 0x66) = *(undefined2 *)((int)unaff_A6 + 0x66);
  }
  return;
}
