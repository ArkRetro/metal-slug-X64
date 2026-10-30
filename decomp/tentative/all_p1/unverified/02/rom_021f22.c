/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021f22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21f22(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  char cVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 0x1e;
  *unaff_A6 = 0x21f34;
  candidate_2223c();
  candidate_427a();
  cVar3 = *(char *)((int)unaff_A6 + 0x82);
  cVar4 = cVar3 + -1;
  *(char *)((int)unaff_A6 + 0x82) = cVar4;
  if (cVar4 == '\0' || cVar3 < '\x01') {
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
