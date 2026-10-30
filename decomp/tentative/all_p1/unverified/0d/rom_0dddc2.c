/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dddc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dddc2(void)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar2 = 0x40000;
  if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
LAB_000dde46:
      candidate_1b1dc();
      candidate_4256();
      *unaff_A6 = 0xdde62;
      uVar3 = 0;
      func_0x000143e4();
      candidate_427a();
      if ((bool)uVar3) {
        *unaff_A6 = 0xddcf4;
        uVar3 = 0;
      }
      goto LAB_000de016;
    }
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) goto LAB_000dde46;
    uVar2 = 0xfffc0000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x14;
  candidate_4256();
  *unaff_A6 = 0xdde0c;
  candidate_1456a();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x82);
  uVar3 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x82) = sVar1 + -1;
  candidate_de33e();
  if ((!(bool)uVar3) &&
     (uVar3 = *(short *)((int)unaff_A6 + 0x82) == 0, *(short *)((int)unaff_A6 + 0x82) < 2)) {
    *unaff_A6 = 0xddcf4;
    uVar3 = 0;
  }
  candidate_de37e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_ddf5a;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_ddf5a;
    uVar3 = 0;
  }
LAB_000de016:
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x76) + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  candidate_1aeda();
  if (!(bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) &&
     (uVar3 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) {
    *unaff_A6 = candidate_de064;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_de06e;
  }
  return;
}
