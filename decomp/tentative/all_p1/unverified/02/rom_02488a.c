/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02488a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2488a(void)

{
  undefined1 uVar1;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x98) < 0xff) {
    *(short *)(unaff_A6 + 0x98) = *(short *)(unaff_A6 + 0x98) + 8;
    if (0xfe < *(short *)(unaff_A6 + 0x98)) {
      *(undefined2 *)(unaff_A6 + 0x98) = 0xff;
    }
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x98) = 0xff;
  }
  uVar1 = (undefined1)*(undefined2 *)(unaff_A6 + 0x98);
  *(undefined1 *)(unaff_A6 + 0x12) = uVar1;
  *(undefined1 *)(unaff_A6 + 0x13) = uVar1;
  candidate_1ac38();
  *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0x4000;
  candidate_427a();
  candidate_24990();
  return;
}
