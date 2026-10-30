/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b090.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b090(void)

{
  short sVar1;
  short extraout_D1u;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_8b31a();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x7f) & 4) == 0) {
    bVar2 = false;
    sVar1 = candidate_1495c();
    if (bVar2) {
      if (*(short *)((int)unaff_A6 + 0x72) <= sVar1) {
        *(undefined1 *)((int)unaff_A6 + 0x7e) = 1;
      }
      if (*(short *)(unaff_A6 + 0x1c) <= sVar1) {
        *unaff_A6 = candidate_8b138;
      }
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = thunk_FUN_0008b18e;
    }
  }
  else {
    candidate_1ac4c();
    if (*(short *)((int)unaff_A6 + 0x72) <= -extraout_D1u) {
      *(undefined1 *)((int)unaff_A6 + 0x7e) = 1;
    }
    if (*(short *)(unaff_A6 + 0x1c) <= -extraout_D1u) {
      *unaff_A6 = candidate_8b138;
    }
  }
  if (*(char *)((int)unaff_A6 + 0x81) != '\0') {
    *unaff_A6 = candidate_8b138;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = 0x8b250;
  }
  return;
}
