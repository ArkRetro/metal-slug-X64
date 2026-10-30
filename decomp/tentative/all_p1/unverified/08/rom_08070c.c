/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08070c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8070c(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  undefined1 extraout_var;
  ushort uVar5;
  int iVar4;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ee;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ee;
  candidate_9ea8();
  candidate_29c6();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x8d) = extraout_var;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 2;
  uVar5 = candidate_29c6();
  *(int *)((int)unaff_A6 + 0x72) = (short)(uVar5 & 0x7fff) + -0xc000;
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xc];
  uVar5 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x86) = (uVar5 & 0xf) + 0x10;
  *unaff_A6 = 0x80786;
  unaff_A6[0xd] = (*(int *)((int)unaff_A6 + 0x72) - unaff_A6[0xd] >> 4) + unaff_A6[0xd];
  *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + '\b';
  iVar4 = candidate_2e7c();
  *(int *)((int)unaff_A6 + 0x76) =
       *(int *)((int)unaff_A6 + 0x76) - (*(int *)((int)unaff_A6 + 0x76) >> 5);
  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x76) + iVar4;
  candidate_1ac38();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
    uVar5 = *(ushort *)((int)unaff_A6 + 0x86);
    uVar2 = uVar5 - 1;
    *(ushort *)((int)unaff_A6 + 0x86) = uVar2;
    bVar6 = -1 < (int)((uint)uVar2 << 0x10);
    if (!bVar6) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
      *(undefined2 *)((int)unaff_A6 + 0x86) = 0x10;
    }
    bVar6 = bVar6 && uVar5 == 0;
  }
  else {
    uVar5 = *(ushort *)((int)unaff_A6 + 0x86);
    bVar6 = uVar5 == 0;
    uVar5 = uVar5 - 1;
    *(ushort *)((int)unaff_A6 + 0x86) = uVar5;
    if ((int)((uint)uVar5 << 0x10) < 0) goto LAB_00080546;
  }
  candidate_19504();
  if (!bVar6) {
    return;
  }
LAB_00080546:
  uVar5 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
  if (0xfc10 < uVar5) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar2 = ~uVar5 & 0xfff;
    uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar5 ^ uVar3) & 1) != 0) {
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
  iVar4 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar4 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
