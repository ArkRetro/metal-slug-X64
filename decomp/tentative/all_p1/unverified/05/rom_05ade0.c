/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05ade0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ade0(void)

{
  uint uVar1;
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  unaff_A6[0xd] = 0x6aaaa;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = 0xffffc71d;
  unaff_A6[0xc] = 0x6aaaa;
  uVar1 = candidate_29c6();
  unaff_A6[0xc] = unaff_A6[0xc] + ((uVar1 & 0x7f) - 0x3f) * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  candidate_4256();
  *unaff_A6 = 0x5ae58;
  candidate_5b72a();
  candidate_427a();
  candidate_5b714();
  return;
}
