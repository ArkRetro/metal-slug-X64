/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a97e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a97e(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined1 *)(unaff_A6 + 0x1f) = 0;
  candidate_146e8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_3fb10();
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
  candidate_4256();
  *unaff_A6 = 0x5a9be;
  candidate_1456a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_5aacc;
  }
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x1f) == '\0') {
    unaff_A6[0x14] = 0x5b74e;
  }
  else {
    unaff_A6[0x14] = 0;
  }
  bVar1 = false;
  candidate_4c24();
  if (bVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *unaff_A6 = candidate_5aa16;
  }
  candidate_5b714();
  return;
}
