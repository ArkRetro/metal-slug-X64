/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06c4e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_6c4e0(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar4;
  short sVar5;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  iVar2 = ((int)-unaff_A6[0xd] >> 3) + unaff_A6[0xd];
  unaff_A6[0xd] = iVar2;
  uVar4 = (ushort)-(iVar2 < 0);
  if ((ushort)((uVar4 ^ *(ushort *)(unaff_A6 + 0xd)) - uVar4) < 3) {
    if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
      unaff_A6[0xc] =
           (int)(short)-((int)((uint)(ushort)(*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0x3c) -
                                             *(short *)(unaff_A6 + 0xf)) << 0x10) < 0);
      *unaff_A6 = 0x6c526;
      uVar4 = *(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0x3c) - *(short *)(unaff_A6 + 0xf);
      if (-1 < (short)(uVar4 ^ *(ushort *)(unaff_A6 + 0xc))) {
        uVar4 = (short)uVar4 >> 0xf;
        unaff_A6[0xc] =
             ((short)((uVar4 ^ *(ushort *)(unaff_A6 + 0x21)) - uVar4) * 0x100 - unaff_A6[0xc] >> 4)
             + unaff_A6[0xc];
        unaff_A6[0xd] = ((int)-unaff_A6[0xd] >> 2) + unaff_A6[0xd];
        goto LAB_0006c5a0;
      }
    }
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 8;
    *unaff_A6 = 0x6c572;
    unaff_A6[0xc] = ((int)-unaff_A6[0xc] >> 2) + unaff_A6[0xc];
    sVar5 = *(short *)(unaff_A6 + 0x21);
    if ((*(byte *)((int)unaff_A6 + 0x8d) & 4) == 0) {
      sVar5 = -sVar5;
    }
    unaff_A6[0xd] = (sVar5 * 0x100 - unaff_A6[0xd] >> 4) + unaff_A6[0xd];
  }
LAB_0006c5a0:
  candidate_1ac38();
  candidate_6c746();
  sVar5 = candidate_49e8();
  if (sVar5 != 0) {
    *unaff_A6 = candidate_6c6bc;
  }
  bVar6 = false;
  uVar3 = candidate_1945c();
  if (bVar6) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (((*(byte *)((int)unaff_A6 + 0x8d) & 8) != 0) &&
     (uVar3 = CONCAT22((short)(uVar3 >> 0x10),*(ushort *)(unaff_A5 + -0x7ff2)) & 0xffff0003,
     (*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0)) {
    bVar6 = false;
    uVar3 = candidate_40bc();
    if (bVar6) {
      uVar3 = candidate_1b220();
    }
  }
  return uVar3;
}
