/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a40c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a40c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  *unaff_A6 = 0x7a428;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_7b730();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7a97a;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_7b996();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7aae2;
  }
  return;
}
