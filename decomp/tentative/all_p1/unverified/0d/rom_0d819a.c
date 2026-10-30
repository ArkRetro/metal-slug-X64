/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d819a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d819a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  candidate_4256();
  *unaff_A6 = 0xd81c4;
  uVar1 = 0;
  candidate_d9276();
  candidate_44f6();
  candidate_427a();
  if ((bool)uVar1) {
    candidate_d92ec();
    uVar1 = 0;
    if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      uVar1 = 0;
      candidate_d922c();
    }
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
