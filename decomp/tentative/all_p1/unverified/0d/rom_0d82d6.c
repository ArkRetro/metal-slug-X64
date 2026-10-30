/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d82d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d82d6(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_4256();
  *unaff_A6 = 0xd82f0;
  candidate_d9276();
  candidate_427a();
  candidate_d92ec();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9b);
    uVar2 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9b) = cVar1 + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + '\x01';
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 0x1e;
    *unaff_A6 = candidate_d84d4;
    uVar2 = 0;
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
