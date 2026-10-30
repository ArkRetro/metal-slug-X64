/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d142.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8d142(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  if ((int)unaff_A6[0xd] < -0x20000) {
    *unaff_A6 = candidate_8d188;
  }
  candidate_1ac38();
  bVar1 = *(byte *)((int)unaff_A6 + 0x72) & 1;
  *(byte *)((int)unaff_A6 + 0x72) = bVar1;
  if (bVar1 == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x72) = *(char *)((int)unaff_A6 + 0x72) + '\x01';
  return;
}
