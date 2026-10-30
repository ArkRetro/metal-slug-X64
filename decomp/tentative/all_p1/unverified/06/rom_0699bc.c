/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0699bc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_699bc(void)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  byte bVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_69cfc();
                    /* WARNING: Switch is manually overridden */
  switch((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) {
  case 0:
    *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
    *unaff_A6 = 0x69a9c;
    iVar3 = *(int *)((int)unaff_A6 + 0x6e);
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xc] = iVar3;
    *(int *)((int)unaff_A6 + 0x62) = iVar3;
    unaff_A6[0xd] = 0;
    uVar5 = 0;
    candidate_1456a();
    candidate_69d4a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_69a46;
      uVar5 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = candidate_699e0;
      uVar5 = 0;
    }
    candidate_69d94();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_69b06;
      uVar5 = 0;
    }
    candidate_69dd0();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_69bf4;
    }
    break;
  case 4:
    *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
    *unaff_A6 = 0x699ec;
    uVar2 = 0x1000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar2 = 0xfffff000;
    }
    unaff_A6[0xc] = uVar2;
    unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x6e);
    uVar5 = 0;
    func_0x000143e4();
    candidate_69d18();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 2) != 0) {
      *unaff_A6 = candidate_69b06;
      uVar5 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
      *unaff_A6 = candidate_69a90;
      uVar5 = 0;
    }
    candidate_69dd0();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_69bf4;
    }
    break;
  case 8:
    *(undefined1 *)((int)unaff_A6 + 0x83) = 2;
    *unaff_A6 = 0x69a52;
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = -*(int *)((int)unaff_A6 + 0x6e);
    uVar5 = 0;
    func_0x000143e4();
    candidate_69d18();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_69a90;
      uVar5 = 0;
    }
    candidate_69dd0();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_69bf4;
    }
  }
  bVar4 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar4) {
    bVar4 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar4;
  uVar5 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar5 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar5 = 0;
  }
  candidate_1945c();
  if ((bool)uVar5) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
