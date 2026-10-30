/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddce6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddce6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x5e) = *(undefined1 *)(*(int *)((int)unaff_A6 + 0x76) + 0x5e);
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0xddd10;
  uVar1 = 0;
  func_0x000143e4();
  candidate_427a();
  thunk_FUN_000c8cfc();
  if (!(bool)uVar1) {
    candidate_de2f2();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_ddd62;
      uVar1 = 0;
    }
    candidate_de2c0();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_dde7c;
      uVar1 = 0;
    }
    candidate_de28e();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_ddf14;
      uVar1 = 0;
    }
    candidate_de33e();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_dddc2;
      uVar1 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_ddf8c;
    uVar1 = 0;
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x76) + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  candidate_1aeda();
  if (!(bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) &&
     (uVar1 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) {
    *unaff_A6 = candidate_de064;
    uVar1 = 0;
  }
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_de06e;
  }
  return;
}
