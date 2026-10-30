/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e398.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1e398(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  undefined2 unaff_D2w;
  ushort uVar5;
  undefined2 unaff_D3w;
  int extraout_A1;
  int unaff_A2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  *(undefined1 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
  if (*(char *)(unaff_A6 + 0x1c) != '\0') goto LAB_0001e412;
  do {
    unaff_A6[0xd] = 0;
    *unaff_A6 = 0x1e3b2;
    uVar6 = 0;
    candidate_1f002();
    candidate_1ec1e();
    candidate_1ed32();
    if ((bool)uVar6) {
LAB_0001e4d4:
      candidate_1e13a();
      *(undefined1 *)(extraout_A1 + 7) = 0;
      if (*(char *)(unaff_A2 + 7) == '\0') {
        candidate_12192();
      }
      uVar5 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
      if (0xfc10 < uVar5) {
        uVar5 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar3 = ~uVar5 & 0xfff;
        uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)
                           ) != 0));
        uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar5 ^ uVar4) & 1) != 0) {
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
          func_0x0003cf12();
          candidate_3e2a8();
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
      }
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    if (*(char *)(unaff_A6 + 0x1c) == '\0') {
      candidate_4256();
      candidate_427a();
      uVar6 = 0;
      candidate_4256();
      candidate_1ed60();
      if ((bool)uVar6) {
        candidate_4256();
        candidate_427a();
      }
      goto LAB_0001e47e;
    }
LAB_0001e412:
    unaff_A6[0xc] = 0;
    *unaff_A6 = 0x1e41c;
    uVar6 = 0;
    candidate_1f002();
    candidate_1ec1e();
    candidate_1ed32();
    if ((bool)uVar6) goto LAB_0001e4d4;
    if (*(char *)(unaff_A6 + 0x1c) != '\0') {
      candidate_4256();
      candidate_427a();
      uVar6 = 0;
      candidate_4256();
      candidate_427a();
      candidate_1edf6();
      if ((bool)uVar6) {
        candidate_4256();
        candidate_427a();
      }
LAB_0001e47e:
      *(char *)((int)unaff_A6 + 0x82) = *(char *)((int)unaff_A6 + 0x82) + '\x01';
      *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + '\x01';
      if ((*(byte *)(unaff_A6 + 0x22) & 2) == 0) {
        if ('\v' < *(char *)((int)unaff_A6 + 0x82)) {
          *unaff_A6 = 0x1e4d4;
        }
      }
      else if ('\x03' < *(char *)((int)unaff_A6 + 0x83)) {
        *unaff_A6 = 0x1e4d4;
      }
      if (0 < *(short *)((int)unaff_A6 + 0x7a)) {
        *unaff_A6 = 0x1e274;
      }
      bVar7 = false;
      candidate_1ed32();
      if (bVar7) {
        *unaff_A6 = 0x1e4d4;
      }
      return;
    }
  } while( true );
}
