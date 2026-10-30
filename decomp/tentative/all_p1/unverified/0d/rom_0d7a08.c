/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7a08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7a08(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)((int)unaff_A6 + 0x86) = 0x80;
  *(undefined2 *)(unaff_A6 + 10) = 0x1040;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0x160;
  *(undefined2 *)(unaff_A6 + 0x24) = *(undefined2 *)(unaff_A6 + 0xb);
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x3c;
  *unaff_A6 = 0xd7a2c;
  if (*(short *)((int)unaff_A6 + 0x76) < 0x1f) {
    candidate_d8f4e();
    *unaff_A6 = 0xd7a40;
  }
  candidate_d8fdc();
  *(undefined1 *)((int)unaff_A6 + 0x89) = 0x80;
  candidate_1ac38();
  candidate_d90d2();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if (*(short *)((int)unaff_A6 + 0x76) < 1) {
    candidate_260c8();
    candidate_d914a();
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  return;
}
