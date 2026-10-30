/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f752.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f752(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_a055c();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xc;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *unaff_A6 = 0x9f76a;
  }
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    candidate_a0334();
    candidate_427a();
    *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) & 0xfb;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 9;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *unaff_A6 = 0x9f782;
    candidate_a0334();
    candidate_427a();
    *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) | 4;
  }
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
    *unaff_A6 = candidate_9f846;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x9fca6;
  }
  bVar1 = false;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
