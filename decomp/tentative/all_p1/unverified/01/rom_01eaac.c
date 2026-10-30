/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01eaac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1eaac(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  ushort uVar4;
  ushort uVar5;
  short sVar6;
  byte bVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;

  do {
    if (*(char *)((int)unaff_A6 + 0x86) < '\x01') {
      *(undefined4 *)((int)unaff_A6 + 0x76) = 0x1f156;
      candidate_1b1dc();
      *unaff_A6 = 0x1eb72;
      candidate_1436a();
      if ((*(int *)((int)unaff_A6 + 0x76) != 0) && (sVar6 = candidate_49e8(), sVar6 != 0)) {
        *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
      }
      bVar9 = false;
      candidate_427a();
      if (bVar9) {
        *unaff_A6 = 0x1e4f2;
      }
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        if (0x13f < *(ushort *)(unaff_A6 + 0xf)) {
          *unaff_A6 = 0x1e4f2;
        }
        if (0x13f < (ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U)) {
          *unaff_A6 = 0x1e4f2;
        }
        cVar3 = *(char *)((int)unaff_A6 + 0x12);
        if (cVar3 != -1) {
          bVar7 = ((byte)(-cVar3 - 1U) >> 2) + cVar3;
          if (0xe0 < bVar7) {
            bVar7 = 0xff;
          }
          *(byte *)((int)unaff_A6 + 0x12) = bVar7;
          *(byte *)((int)unaff_A6 + 0x13) = bVar7;
          return;
        }
        return;
      }
candidate_4188:
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    candidate_1456a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
      if (0xfc10 < uVar8) {
        uVar8 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar5 = uRam002fffe8;
        uVar4 = ~uVar8 & 0xfff;
        uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)
                           ) != 0));
        uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar8 ^ uVar5) & 1) != 0) {
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
      goto candidate_4188;
    }
    *(char *)((int)unaff_A6 + 0x86) = *(char *)((int)unaff_A6 + 0x86) + -1;
  } while( true );
}
