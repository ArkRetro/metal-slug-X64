/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07dbb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7dbb6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x7dbcc;
  candidate_1abc8();
  uVar1 = *(char *)(unaff_A6[3] + 0x60) != -1;
  if (!(bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x26) = 0x80;
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7dc0c;
    uVar1 = 0;
  }
  candidate_7dcbc();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7dc7a;
  }
  return;
}
