/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a14fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_a14fe(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 uVar8;
  ushort uVar9;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;

  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0;
  uVar8 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar8;
  *(undefined2 *)(unaff_A6 + 0x20) = uVar8;
  uVar8 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar8;
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_4256();
  *unaff_A6 = 0xa1578;
  bVar10 = false;
  candidate_a24f8();
  if (!bVar10) {
    bVar5 = *(byte *)(unaff_A6[3] + 0x60);
    if (bVar5 == 0xfe) {
      *(undefined1 *)((int)unaff_A6 + 0x86) = 3;
    }
    bVar10 = bVar5 < 0xfe;
    if ((*(byte *)(unaff_A6[3] + 0x92) & 1) == 0) {
      candidate_4c24();
      if (bVar10) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        iVar2 = unaff_A6[3];
        *(undefined1 *)(iVar2 + 0x8c) = *(undefined1 *)((int)unaff_A6 + 0x17);
        *(byte *)(iVar2 + 0x90) = *(byte *)((int)unaff_A6 + 0x5e) | *(byte *)(iVar2 + 0x90);
        *(undefined1 *)((int)unaff_A6 + 0x5e) = 0;
        sVar3 = *(short *)(unaff_A6 + 0x20);
        sVar4 = *(short *)((int)unaff_A6 + 0x4e);
        *(short *)(unaff_A6 + 0x20) = sVar4;
        if (*(short *)(iVar2 + 0x80) < (short)(sVar3 - sVar4)) {
          *(short *)(iVar2 + 0x80) = sVar3 - sVar4;
        }
        if ((*(char *)((int)unaff_A6 + 0x86) < '\x03') &&
           (sVar4 <= *(short *)((int)unaff_A6 + 0x7e))) {
          *(char *)((int)unaff_A6 + 0x86) = *(char *)((int)unaff_A6 + 0x86) + '\x01';
          if (*(char *)((int)unaff_A6 + 0x86) == '\x01') {
            uVar8 = candidate_260c8();
          }
          else {
            uVar8 = candidate_260c8();
          }
          *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar8;
          *(undefined1 *)(unaff_A6[3] + 0x8d) = 0x30;
          if (*(char *)((int)unaff_A6 + 0x8d) < '%') {
            *(undefined1 *)((int)unaff_A6 + 0x8d) = 0x30;
            candidate_a2718();
          }
          candidate_120f8();
        }
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
          *(undefined2 *)((int)unaff_A6 + 0x4e) = 100;
          *(undefined2 *)(unaff_A6 + 0x20) = 100;
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
        }
      }
    }
    if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
      *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + -1;
    }
    candidate_1abc8();
    candidate_a25e0();
    return;
  }
  uVar9 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
  if (0xfc10 < uVar9) {
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar6 = ~uVar9 & 0xfff;
    uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar6 & 7)) !=
                      0));
    uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar6));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar9 ^ uVar7) & 1) != 0) {
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
