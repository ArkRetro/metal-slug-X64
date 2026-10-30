/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0580a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_580a2(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  candidate_29c6();
  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0;
  candidate_1abc8();
  *unaff_A6 = 0x580ee;
  bVar1 = *(byte *)((int)unaff_A6 + 0x12);
  *(byte *)((int)unaff_A6 + 0x12) = bVar1 + 0x10;
  if (0xef < bVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
    *(undefined1 *)(unaff_A6[3] + 0x78) = 0xff;
    *unaff_A6 = 0x5810a;
  }
  candidate_427a();
  return;
}
