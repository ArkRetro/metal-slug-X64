/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0904b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_904b0(void)

{
  short sVar1;
  undefined1 uVar3;
  int iVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_914a2();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  bVar4 = false;
  candidate_919f4();
  if (!bVar4) {
    candidate_1af9a();
    *(undefined4 *)((int)unaff_A6 + 0x76) = extraout_A0;
    *(undefined1 *)(unaff_A6 + 0x18) = 2;
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    bVar4 = false;
    uVar3 = candidate_919f4();
    if (!bVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar3;
      candidate_4256();
      candidate_91ab6();
      *unaff_A6 = 0x9051c;
      candidate_91494();
      candidate_427a();
      if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
        *unaff_A6 = candidate_90538;
      }
      uVar3 = false;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
      candidate_4c24();
      if ((bool)uVar3) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        uVar3 = 0;
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x40;
      }
      goto LAB_000908c0;
    }
  }
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
  uVar3 = 0;
  candidate_91416();
  candidate_919f4();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_90124;
    uVar3 = 0;
  }
  candidate_427a();
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar1 = *(short *)(unaff_A6 + 0x20);
  bVar4 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x20) = sVar1 + -1;
  candidate_919a4();
  if (bVar4) {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) ^ 0x80;
    *unaff_A6 = candidate_90772;
    uVar3 = false;
  }
  else if ((*(short *)(unaff_A6 + 0xf) < 0x10) && ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0)) {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x80;
    *unaff_A6 = candidate_90772;
    uVar3 = false;
  }
  else {
    uVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x130;
    if ((0x130 < (short)*(ushort *)(unaff_A6 + 0xf)) &&
       ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0)) {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0x7f;
      *unaff_A6 = candidate_90772;
      uVar3 = false;
    }
  }
  candidate_91ada();
LAB_000908c0:
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar3 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
