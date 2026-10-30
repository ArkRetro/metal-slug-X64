/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0949ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_949ea(void)

{
  undefined2 uVar1;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    uVar1 = candidate_14c48();
    *(undefined2 *)(unaff_A6 + 0x84) = uVar1;
    return;
  }
  *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)(unaff_A6 + 0x62);
  *(int *)(unaff_A6 + 0x34) =
       (-0x5c28c0 - *(int *)(unaff_A6 + 0x34) >> 6) + *(int *)(unaff_A6 + 0x34);
  uVar1 = candidate_14c0c();
  *(undefined2 *)(unaff_A6 + 0x84) = uVar1;
  return;
}
