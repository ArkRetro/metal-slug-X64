/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x090538.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_90538(void)

{
  short sVar1;
  undefined1 uVar4;
  undefined2 uVar3;
  int iVar2;
  undefined1 uVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  uVar4 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x23) = uVar4;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    uVar4 = 3;
    uVar5 = 0xff;
  }
  else {
    uVar4 = 4;
    uVar5 = 0;
  }
  *(undefined1 *)(unaff_A6 + 0x18) = uVar4;
  *(undefined1 *)((int)unaff_A6 + 0x95) = uVar5;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  uVar3 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x20) = uVar3;
  bVar6 = false;
  uVar4 = candidate_919f4();
  if (bVar6) {
    iVar2 = *(int *)((int)unaff_A6 + 0x6e);
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
      iVar2 = -iVar2;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar2;
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    unaff_A6[0x14] = 0x91cec;
    candidate_4e76();
    candidate_4256();
    *unaff_A6 = 0x906da;
    uVar4 = 0;
    candidate_91416();
    candidate_919f4();
    if (!(bool)uVar4) {
      *unaff_A6 = candidate_90124;
      uVar4 = 0;
    }
    candidate_427a();
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    sVar1 = *(short *)(unaff_A6 + 0x20);
    bVar6 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x20) = sVar1 + -1;
    candidate_919a4();
    if (bVar6) {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) ^ 0x80;
      *unaff_A6 = candidate_90772;
      uVar4 = false;
    }
    else if ((*(short *)(unaff_A6 + 0xf) < 0x10) && ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0))
    {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x80;
      *unaff_A6 = candidate_90772;
      uVar4 = false;
    }
    else {
      uVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x130;
      if ((0x130 < (short)*(ushort *)(unaff_A6 + 0xf)) &&
         ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0)) {
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0x7f;
        *unaff_A6 = candidate_90772;
        uVar4 = false;
      }
    }
    candidate_91ada();
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
    candidate_4256();
    candidate_91ab6();
    *unaff_A6 = 0x905c2;
    candidate_91494();
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
      *(undefined1 *)(unaff_A6 + 0x18) = 6;
    }
    *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
    if (*(short *)(unaff_A6 + 0x20) < 1) {
      *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + -1;
      if (*(char *)(unaff_A6 + 0x23) < '\x01') {
        *unaff_A6 = candidate_9060e;
      }
      else {
        *unaff_A6 = 0x90548;
      }
    }
    uVar4 = false;
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar4 = 0;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x40;
    }
  }
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar4 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
