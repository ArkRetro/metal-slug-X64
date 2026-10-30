/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a1756.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a1756(void)

{
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  unaff_A6[0xd] = 0x44444;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0xffffb72f;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0xc] = 0x22222;
  }
  else {
    unaff_A6[0xc] = 0xfffdddde;
  }
  if (*(char *)(unaff_A6 + 0x18) < '\b') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0xa17e2;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  candidate_1abc8();
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xb) < 0x21) {
    candidate_9f88();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
    *unaff_A6 = candidate_1c71e;
  }
  if (*(short *)(unaff_A6 + 0x10) < 0x81) {
    *unaff_A6 = candidate_a1466;
  }
  return;
}
