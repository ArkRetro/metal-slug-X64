/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06107e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6107e(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x61098;
  if (*(char *)((int)unaff_A6 + 0x61) != '\x03') {
    candidate_611fa();
    candidate_427a();
    candidate_611e2();
    return;
  }
  uVar1 = candidate_29c6();
  if ((uVar1 & 1) != 0) {
    candidate_4256();
    *unaff_A6 = 0x610d6;
    uVar2 = 0;
    candidate_611fa();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_41ac;
    }
    candidate_611e2();
    return;
  }
  candidate_4256();
  *unaff_A6 = 0x61106;
  uVar2 = 0;
  candidate_611fa();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  candidate_611e2();
  return;
}
