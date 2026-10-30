/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a241a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a241a(void)

{
  uint uVar1;
  ushort uVar3;
  int iVar2;
  ushort extraout_D1w;
  int unaff_A6;

  if (0x1f < *(short *)(unaff_A6 + 0x3c)) {
    if (*(short *)(unaff_A6 + 0x3c) < 0x101) {
      candidate_a2330();
      uVar3 = candidate_a2354();
      if ((uVar3 & 0x7f) < (extraout_D1w & 0x7f)) goto LAB_000a24a8;
      if ((uVar3 & 0x7f) <= (extraout_D1w & 0x7f)) {
        iVar2 = *(int *)(unaff_A6 + 0x30);
        if (iVar2 < 0) {
          iVar2 = -iVar2;
        }
        if (iVar2 < (int)(*(uint *)(unaff_A6 + 0x62) >> 4)) {
          *(undefined4 *)(unaff_A6 + 0x30) = 0;
          return;
        }
        if (*(int *)(unaff_A6 + 0x30) < 0) goto LAB_000a24a8;
      }
    }
    uVar1 = *(uint *)(unaff_A6 + 0x62);
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) - (uVar1 >> 4);
    if (-*(int *)(unaff_A6 + 0x30) != uVar1 && *(int *)(unaff_A6 + 0x30) <= (int)-uVar1) {
      *(uint *)(unaff_A6 + 0x30) = -uVar1;
    }
    return;
  }
LAB_000a24a8:
  uVar1 = *(uint *)(unaff_A6 + 0x62);
  *(int *)(unaff_A6 + 0x30) = (uVar1 >> 4) + *(int *)(unaff_A6 + 0x30);
  if ((int)uVar1 < *(int *)(unaff_A6 + 0x30)) {
    *(uint *)(unaff_A6 + 0x30) = uVar1;
  }
  return;
}
