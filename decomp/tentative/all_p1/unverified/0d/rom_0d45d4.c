/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d45d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d45d4(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x92) = 4;
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xf7;
  *unaff_A6 = candidate_d455a;
  uVar2 = 0;
  candidate_d5506();
  candidate_d54ee();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0xd44ca;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_d45ee;
  }
  cVar1 = *(char *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  *(undefined1 *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e)) = 0;
  if (cVar1 == '\x05') {
    *unaff_A6 = candidate_d470e;
  }
  return;
}
