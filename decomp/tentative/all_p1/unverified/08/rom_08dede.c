/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08dede.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8dede(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  bVar1 = false;
  candidate_8f4a0();
  if (bVar1) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x8df16;
  uVar2 = false;
  candidate_8f51c();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
    *unaff_A6 = candidate_8de84;
    uVar2 = 0;
  }
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_8ddfa;
    uVar2 = 0;
  }
  candidate_8f3b6();
  candidate_8e15a();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar2 = 0;
  }
  candidate_8f432();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
