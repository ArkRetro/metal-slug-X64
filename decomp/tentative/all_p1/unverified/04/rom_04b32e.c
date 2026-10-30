/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b32e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b32e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x26);
  *unaff_A6 = 0x4b352;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x9e) != '\0') {
    *(undefined1 *)(unaff_A6[3] + 0x9e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0;
  }
  bVar3 = false;
  candidate_4ddd4();
  if (bVar3) {
    if (*(char *)((int)unaff_A6 + 0x9d) == '\x04') {
      *unaff_A6 = candidate_4b32e;
    }
    if (*(char *)((int)unaff_A6 + 0x9d) == '\x03') {
      *unaff_A6 = candidate_4b2f6;
    }
  }
  iVar2 = unaff_A6[3];
  *(undefined2 *)(iVar2 + 0x98) = *(undefined2 *)(unaff_A6 + 0x26);
  if (*(char *)(iVar2 + 0x9d) == '\x7f') {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
