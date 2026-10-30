/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d231a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d231a(void)

{
  undefined4 *unaff_A6;

  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xfb;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xef;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  *unaff_A6 = 0xd233a;
  candidate_d3220();
  candidate_d3268();
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x60) == 'w') {
    *unaff_A6 = candidate_d235e;
  }
  return;
}
