/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x075d5e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_75d5e(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  undefined1 extraout_var;
  char cVar6;
  ushort uVar5;
  int iVar4;
  int unaff_D2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  undefined8 uVar8;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  candidate_9ea8();
  if (*(short *)(unaff_A6 + 0x2a) < 1) {
    *(undefined2 *)(unaff_A6 + 0x2a) = 0x400;
  }
  candidate_29c6();
  uVar8 = candidate_2e7c();
  unaff_A6[0xc] = (int)((ulonglong)uVar8 >> 0x20);
  unaff_A6[0xd] = (int)uVar8 << 1;
  cVar6 = candidate_29c6();
  if (cVar6 < '\x01') {
    uVar5 = candidate_29c6();
    uVar5 = (uVar5 & 3) + 1;
    unaff_A6[0xc] =
         (int)((unaff_A6[0xc] & 0xffff) * (uint)uVar5 +
              (int)(short)uVar5 * (int)(short)((uint)unaff_A6[0xc] >> 0x10) * 0x10000) >> 1;
    uVar5 = candidate_29c6();
    uVar5 = (uVar5 & 3) + 1;
    unaff_D2 = (unaff_A6[0xd] & 0xffff) * (uint)uVar5;
    unaff_A6[0xd] =
         unaff_D2 + (int)(short)uVar5 * (int)(short)((uint)unaff_A6[0xd] >> 0x10) * 0x10000 >> 1;
  }
  iVar4 = unaff_A6[0xd];
  if (0xc0000 < iVar4) {
    iVar4 = 0xc0000;
  }
  unaff_A6[0x1e] = iVar4;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0xffe0;
  *unaff_A6 = candidate_75f64;
  uVar7 = 0;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *(undefined2 *)(unaff_A6 + 0x1d) = 0xffe0;
    unaff_A6[0xc] = (int)unaff_A6[0xc] >> 2;
    unaff_A6[0xd] = (int)unaff_A6[0x1e] >> 1;
    unaff_A6[0x1e] = (int)unaff_A6[0x1e] >> 1;
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    uVar5 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
    if (0xfc10 < uVar5) {
      uVar5 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar5 & 0xfff;
      uVar5 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar2 & 7)) != 0));
      uRam002fffe0 = uVar5 & CONCAT11(extraout_var,-(0x7ff < uVar2));
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
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar7) {
    *unaff_A6 = 0x765d4;
  }
  return;
}
