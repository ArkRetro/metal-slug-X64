/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d4de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8d4de(void)

{
  char *pcVar1;
  code *pcVar2;
  int iVar3;
  undefined4 uVar4;
  ushort uVar5;
  ushort uVar6;
  short sVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_8d814();
  if (!(bool)in_CF) {
    candidate_1abc8();
    if ((*(byte *)(unaff_A6 + 0x28) & 1) == 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
    *(char *)(unaff_A6 + 0x28) = *(char *)(unaff_A6 + 0x28) + '\x01';
    sVar7 = 0;
    uVar4 = unaff_A6[10];
    do {
      if (*(char *)((int)unaff_A6 + sVar7 + 0x91) == '\0') {
        if (*(char *)((int)unaff_A6 + sVar7 + 0x96) == '\0') {
          iVar3 = *(int *)((short)((ushort)*(byte *)((int)unaff_A6 + sVar7 + 0x9b) << 2) + 0xe55ec);
          unaff_A6[0xd] = iVar3;
          if (iVar3 == -0x80000000) {
            if ((short)(sVar7 + 1) == *(short *)((int)unaff_A6 + 0x72)) {
              *unaff_A6 = candidate_8d62e;
            }
          }
          else {
            pcVar1 = (char *)((int)unaff_A6 + sVar7 + 0x9b);
            *pcVar1 = *pcVar1 + '\x01';
          }
        }
        else {
          unaff_A6[0xd] = 0x50000;
          pcVar1 = (char *)((int)unaff_A6 + sVar7 + 0x96);
          *pcVar1 = *pcVar1 + -1;
        }
        candidate_8d7f4();
        sVar7 = candidate_8d7b4();
      }
      else {
        pcVar1 = (char *)((int)unaff_A6 + sVar7 + 0x91);
        *pcVar1 = *pcVar1 + -1;
      }
      sVar7 = sVar7 + 1;
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 8;
    } while (sVar7 != *(short *)((int)unaff_A6 + 0x72));
    unaff_A6[10] = uVar4;
    return;
  }
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar5 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) !=
                      0));
    uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0) {
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
  iVar3 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar2 = (code *)*unaff_A6;
    *(undefined4 *)(iVar3 + -4) = 0x424a;
    (*pcVar2)();
  } while( true );
}
