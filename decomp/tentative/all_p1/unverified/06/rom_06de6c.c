/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06de6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6de6c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_6e2b6();
  candidate_1b1dc();
  candidate_6e29c();
  *unaff_A6 = 0x6de84;
  uVar1 = 0;
  candidate_6e258();
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_6df7e;
    uVar1 = 0;
  }
  candidate_6e27a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x6de06;
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8080;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
